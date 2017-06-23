<?php

function lab_migration_edit_contact($form, &$form_state) {
  global $user;
  $form['username'] = array(
    '#type' => 'item',
    '#title' => t('Username'),
    '#markup' => $user->name,
  );

  $form['phone'] = array(
    '#type' => 'textfield', 
    '#title' => t('Phone Number'),
  );

  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
  );

  $form['address'] = array(
    '#type' => 'textarea',
    '#title' => t('Address'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Update details',
  );

  return $form;
}

function lab_migration_edit_contact_submit($form, &$form_state) {
  global $user;
  $query = db_query('SELECT * FROM {lab_migration_contact_details} WHERE user_id = (:uid)', array(':uid' => $user->uid));

  if($query->rowCount() > 0) {
    db_update('lab_migration_contact_details')
      ->fields(array(
        'phone' => $form_state['values']['phone'],
        'email_id' => $form_state['values']['email'],
        'address' => $form_state['values']['address'],
      ))
      ->condition('user_id', $user->uid)
      ->execute();
  }
  else {
    db_insert('lab_migration_contact_details')
      ->fields(array(
        'user_id' => $user->uid,
        'phone' => $form_state['values']['phone'],
        'email_id' => $form_state['values']['email'],
        'address' => $form_state['values']['address'],
      ))
      ->execute();
  }

  drupal_goto('lab_migration/form_post');
}