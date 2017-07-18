<?php
/**
 * @file
 * Function to lock code_submission and journal interface.
 */

function lock_interface() {
  global $user;
  $proposal_data = lab_migration_get_proposal();
  if (!$proposal_data) {
    drupal_goto('');
    return;
  }
  
  //var_dump($proposal_data->solution_provider_uid); die;
  $query = db_query("SELECT * FROM {lab_migration_lock} WHERE proposal_id = (:pid) and solution_provider_uid = (:spu)", array(
    ':pid' => $proposal_data->id,
    ':spu' => $proposal_data->solution_provider_uid
  ));
  if ($query->rowCount() > 0) {
    db_update('lab_migration_lock')->fields(array(
      'code_interface' => 0
    ))->condition('proposal_id', $proposal_data->id)->execute();
  }
  
  else {
    db_insert('lab_migration_lock')->fields(array(
      'proposal_id' => $proposal_data->id,
      'solution_provider_uid' => $proposal_data->solution_provider_uid,
      'code_interface' => 0,
      'journal_interface' => 0
    ))->execute();
  }
  
  drupal_goto("lab_migration/code");
}