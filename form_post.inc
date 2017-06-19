<?php
/**
 * @file
 * Track status of forms sent by post.
 */

function lab_migration_form_post_form($form, &$form_state) {
  $proposal_data = lab_migration_get_proposal();
  if(!$proposal_data) {
    drupal_goto('');
    return;
  }
  $query = db_query('SELECT * FROM {lab_migration_form_post} WHERE proposal_id = (:pid)', array(':pid' => $proposal_data->id));
  if($query->rowCount() > 0) {
    drupal_set_message(t('Form post details already uploaded for this proposal.'), 'status');
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
              'copyright' => $form_state['values']['copyright'],
              'receipt' => $form_state['values']['receipt'],
              'undertaking_teacher' => $form_state['values']['undertaking_teacher'],
              'undertaking_hod' => $form_state['values']['undertaking_hod'],
              'undertaking_principal' => $form_state['values']['undertaking_principal'],
              'application' => $form_state['values']['application'],
              'post_service' => $form_state['values']['post_service'],
              'tracking_details' => $form_state['values']['tracking_details'],
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
  drupal_set_message(t('Post details received'), 'status');
}

function tracking_details_ajax($form, &$form_state) {
  return $form['tracking_details'];
}