<?php
/**
 * @file
 *Handles the upload of journal file.
 *
 *Interface to upload lab journal in PDF format. 
 */

function lab_migration_upload_journal_form($form, &$form_state) {
  global $user;
  $proposal_data = lab_migration_get_proposal();
  //error_log(var_export($proposal_data, TRUE));

  if (!$proposal_data) {
    drupal_goto('');
    return;
  }

  $query = db_query("SELECT * FROM {lab_migration_lock} WHERE proposal_id = (:proposal_id) AND solution_provider_uid = (:spu)", array(':proposal_id' => $proposal_data->id, ':spu' => $user->uid));
  
  if($query->rowCount() == 0) {
    drupal_set_message(t('The journal interface is locked for this lab.'), 'status');
     return $form;
  }

  else {
    $val = $query->fetchObject();
    if($val->journal_interface == 0) {
      drupal_set_message(t('The journal interface is locked for this lab.'), 'status');
      return $form;
    }
  }

  $form['#attributes'] = array(
    'enctype' => 'multipart/form-data',
  );
  
  $form['lab_title'] = array(
    '#type' => 'item',
    '#markup' => $proposal_data->lab_title,
    '#title' => t('Title of the Lab'),
  );
  
  $form['name'] = array(
    '#type' => 'item',
    '#markup' => $proposal_data->name_title . ' ' . $proposal_data->name,
    '#title' => t('Proposer Name'),
  );

  $query = db_query('SELECT * FROM {lab_migration_journal} WHERE proposal_id = (:proposal_id)', array(':proposal_id' => $proposal_data->id));

  if ($query->rowCount() != 0) {
    drupal_set_message(t('Journal already uploaded for this lab.'), 'error');
    $val = $query->fetchObject();
    $form['journal'] = array(
      '#type' => 'item',
      '#title' => t('Journal File'),
      '#markup' => l($val->filename, 'lab_migration/download/journal/' . $val->id),
    );
    return $form;
  }
  
  else {
    $form['journal'] = array(
      '#type' => 'file',
      '#title' => t('Upload Journal File'),
      '#description' => t('No spaces or any special characters allowed in filename.') . '<br/>' . t('Allowed file extensions : ') . variable_get('lab_migration_journal_extensions', ''),
    );
  
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );
  
    return $form;
  }
}

function lab_migration_upload_journal_form_validate($form, &$form_state) {
  global $user;
  $proposal_data = lab_migration_get_proposal();
  if (!$proposal_data) {
    drupal_goto('');
    return;
  }

  $allowed_extensions_str = variable_get('lab_migration_journal_extensions', '');

  $allowed_extensions = explode(',', $allowed_extensions_str);
  $temp_extension = end(explode('.', strtolower($_FILES['files']['name']['journal'])));

  if (!in_array($temp_extension, $allowed_extensions))
    form_set_error('journal', t('Only file with ' . $allowed_extensions_str . ' extensions can be uploaded.'));

  if($_FILES['files']['size']['journal'] <= 0)
    form_set_error('journal', t('File size cannot be zero.'));
  if(!lab_migration_check_valid_filename($_FILES['files']['name']['journal']))
    form_set_error('journal', t('Invalid file name specified. Only alphabets and numbers are allowed as a valid filename.'));
}

function lab_migration_upload_journal_form_submit($form, &$form_state) {
 global $user;
  $proposal_data = lab_migration_get_proposal();
  if (!$proposal_data) {
    drupal_goto('');
    return;
  }


  $root_path = lab_migration_path();
  $proposal_id = $proposal_data->id;
  
  $query = db_query('SELECT * FROM {lab_migration_journal} WHERE proposal_id = (:proposal_id)', array(':proposal_id' => $proposal_id));
  if($query->rowCount() != 0) 
    drupal_set_message(t('Journal already uploaded for this lab'), 'error');

  $dest_path = $proposal_id . '/';
  if (!is_dir($root_path . $dest_path)) {
    mkdir($root_path . $dest_path);
  }
  
  $dest_path .= 'JOURNAL' . '/';
  if (!is_dir($root_path . $dest_path)) {
    mkdir($root_path . $dest_path);
    drupal_set_message(t('Journal folder created'), 'status');
  }


    if (file_exists($root_path . $dest_path . $_FILES['files']['name']['journal'])) {
      drupal_set_message(t('Error uploading file. File already exists.'), 'error');
      return;
    }

    //var_dump($_FILES); die;

    if(move_uploaded_file($_FILES['files']['tmp_name']['journal'], $root_path . $dest_path . $_FILES['files']['name']['journal'])){
        db_insert('lab_migration_journal')
          ->fields(array(
            'proposal_id' => $proposal_id,
            'filename' => $_FILES['files']['name']['journal'],
            'filepath' => $dest_path . $_FILES['files']['name']['journal'],
            'filesize' => $_FILES['files']['size']['journal'],
            'filemime' => $_FILES['files']['type']['journal'],
            ))
          ->execute();

      drupal_set_message(t('File uploaded successfully'), 'status');
      $user_data = user_load($proposal_data->uid);
      $email_to = $user_data->mail;
      $param['journal_uploaded']['journal_name'] = $_FILES['files']['name']['journal'];
      $param['journal_uploaded']['user_id'] = $user->uid;
      drupal_mail('lab_migration', 'journal_uploaded', $email_to, language_default(), $param, variable_get('lab_migration_from_email', NULL), TRUE);
    }

    else
      drupal_set_message(t('Error uploading file.'), 'error');
}
