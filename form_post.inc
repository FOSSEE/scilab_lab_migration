<?php
/**
 * @file
 * Track status of forms sent by post.
 */

function lab_migration_form_post_form($form, &$form_state) {
  global $user;
  $proposal_data = lab_migration_get_proposal();
  if(!$proposal_data) {
    drupal_goto('');
    return;
  }
  $query = db_query('SELECT * FROM {lab_migration_form_post} WHERE proposal_id = (:pid)', array(':pid' => $proposal_data->id))->fetchObject();
  $contact_data = db_query('SELECT * FROM {lab_migration_contact_details} WHERE user_id = (:uid)', array(':uid' => $user->uid))->fetchObject();
  if($query != "") {
    $cheque_data = $cheque_data = db_query("SELECT * FROM {lab_migration_cheque} WHERE form_id = (:fid)", array(':fid' => $query->id))->fetchObject();

    drupal_set_message(t('Form post details already uploaded for this proposal.'), 'status');
    $header_row = array('Form Name', 'Status');
    $rows = array(
      array('Copyright Transfer Form', $query->copyright == 0? 'Not Sent':'Sent'),
      array('Receipt', $query->receipt == 0? 'Not Sent':'Sent'),
      array('Undertaking - Teacher', $query->undertaking_teacher == 0? 'Not Sent':'Sent'),
      array('Undertaking - HOD', $query->undertaking_hod == 0? 'Not Sent':'Sent'),
      array('Undertaking - Principal', $query->undertaking_principal == 0? 'Not Sent':'Sent'),
      array('Lab Migration Application Form', $query->application == 0? 'Not Sent':'Sent'),
    );

    $form['docs_sent'] = array(
      '#type' => 'item',
      '#title' => t('Documents Sent by User'),
      '#markup' => theme('table', array('header' => $header_row, 'rows' => $rows)),
    );

    $form['status'] = array(
      '#type' => 'item',
      '#title' => t('Status'),
      '#markup' => $query->status == 0?"Not Received":"Received",
    );

    $form['contact_details'] = array(
    '#type' => 'item',
    '#title' => t('Contact Details'), 
    '#markup' => t("Phone: @phone <br> Email: @email_id <br> Address: @address", array('@phone' => $contact_data->phone, '@email_id' => $contact_data->email_id, '@address' => $contact_data->address)),
    );

    $form['edit_contact'] = array(
      '#type' => 'markup',
      '#markup' => l(t('Edit Contact Details'), "lab_migration/edit_contact/"),
    );

    if (is_null($cheque_data) || $cheque_data->cheque_status == 0) {
      $form['cheque_status'] = array(
        '#type' => 'item',
        '#title' => 'Cheque Not Dispatched'
      );
    }
    else {
      $form['cheque_status'] = array(
        '#type' => 'item',
        '#title' => 'Cheque Dispatched'
      );

      $form['cheque_details'] = array(
        '#type' => 'item',
        '#title' => t('Cheque Details'),
        '#markup' => t("Cheque No. : @cheque_number<br>Amount : @amount<br>Dated : @date<br>Bank : @bank", array(
          '@cheque_number' => $cheque_data->cheque_number,
          '@amount' => $cheque_data->cheque_amount,
          '@date' => date('d-m-Y', strtotime($cheque_data->cheque_date)),
          '@bank' => $cheque_data->cheque_bank,
        )),
      );
    }

    return $form;
  }

  
  $form['instructions'] = array(
    '#type' => 'item',
    '#title' => t('Instructions'),
    '#markup' => t('Mark forms which you have enclosed in the post.')
  );

  $form['copyright'] = array(
    '#type' => 'checkbox',
    '#title' => l(t('Copyright transfer form'), "sites/default/files//Copyright_transfer_form.pdf"),
  );

  $form['receipt'] = array(
    '#type' => 'checkbox',
    '#title' => l(t('Receipt'), "sites/default/files//Receipt1.pdf"),
  );

  $form['undertaking_teacher'] = array(
    '#type' => 'checkbox',
    '#title' => l(t('Undertaking - Teacher'), "sites/default/files//Undertaking_Form.pdf"),
  );

  $form['undertaking_hod'] = array(
    '#type' => 'checkbox',
    '#title' => l(t('Undertaking - HOD'), "sites/default/files//Undertaking_HOD.pdf"),
  );

  $form['undertaking_principal'] = array(
    '#type' => 'checkbox',
    '#title' => l(t('Undertaking - Principal'), "sites/default/files//Undertaking_Principal.pdf"),
  );

  $form['application'] = array(
    '#type' => 'checkbox',
    '#title' => l(t('Lab Migration Application Form'), "/sites/default/files//LMApplication_0.pdf"),
  );

  $query = db_query("SELECT * FROM {postal_services}");
  $services = array();
  while($val = $query->fetchObject()) {
    $services[$val->id] = l(t($val->service_name), $val->tracking_url);
  }

  $form['post_service'] = array(
    '#type' => 'radios',
    '#title' => t('Select your postal/courier service.'),
    '#options' => $services,
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => 'tracking_details_ajax',
      'wrapper' => 'tracking-details-div',
      'method' => 'html',
      'effect' => 'fade',
    ),
  );

  $form['tracking_details'] = array(
    '#prefix' => '<div id="tracking-details-div">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );

  if(isset($form_state['values']['post_service'])) {
    // var_dump('recched here'); die;
    $form['tracking_details'] = array(
      '#type' => 'textfield',
      '#title' => 'Enter tracking details',
      '#size' => 30,
      '#prefix' => '<div id="tracking-details-div">',
      '#suffix' => '</div>',
      '#tree' => TRUE,
      '#required' => TRUE,
    );
  }

  $form['contact_details'] = array(
    '#type' => 'item',
    '#title' => t('Contact Details'), 
    '#markup' => t("Phone: @phone
Email: @email_id
Address: @address", array('@phone' => $contact_data->phone, '@email_id' => $contact_data->email_id, '@address' => $contact_data->address)),
  );

  $form['edit_contact'] = array(
    '#type' => 'markup',
    '#markup' => l(t('Edit Contact Details'), "lab_migration/edit_contact/" . $user->uid),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send Email'),
  );

  return $form;
}

function lab_migration_form_post_form_submit($form, &$form_state) {
  global $user;
  $proposal_data = lab_migration_get_proposal();
  if(!$proposal_data) {
    drupal_goto('');
    return;
  }

  $query = db_insert('lab_migration_form_post')
            ->fields(array(
              'proposal_id' => $proposal_data->id,
              'user_id' => $user->uid,
              'copyright' => $form_state['values']['copyright'],
              'receipt' => $form_state['values']['receipt'],
              'undertaking_teacher' => $form_state['values']['undertaking_teacher'],
              'undertaking_hod' => $form_state['values']['undertaking_hod'],
              'undertaking_principal' => $form_state['values']['undertaking_principal'],
              'application' => $form_state['values']['application'],
              'post_service' => $form_state['values']['post_service'],
              'tracking_details' => $form_state['values']['tracking_details'],
              'status' => 0,
            ))
            ->execute();

  $query = db_query('SELECT * FROM {lab_migration_form_post} WHERE proposal_id = (:pid)', array(':pid' => $proposal_data->id));
  $val = $query->fetchObject();
  $param['form_post']['id'] = $val->id;
  $param['form_post']['uid'] = $user->uid;

  $email_to = $user->mail;
  if (!drupal_mail('lab_migration', 'form_post', $email_to, user_preferred_language($user), $param, variable_get('lab_migration_from_email', NULL), TRUE))
        drupal_set_message(t('Error sending email message.'), 'error');

  $email_to = variable_get('lab_migration_emails', '');
  if (!drupal_mail('lab_migration', 'form_post', $email_to, user_preferred_language($user), $param, variable_get('lab_migration_from_email', NULL), TRUE));
        drupal_set_message(t('Error sending email message.'), 'error');
  drupal_goto('lab_migration/form_post');
  drupal_set_message(t('Post details receifved'), 'status');
}

function tracking_details_ajax($form, &$form_state) {
  return $form['tracking_details'];
}