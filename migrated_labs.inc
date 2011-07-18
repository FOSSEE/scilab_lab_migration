<?php
// $Id$

function migrated_labs()
{
  global $user;
  $dl_root_path = 'sites/default/files/lab_migration/';

  /* get pending proposals to be approved */
  $labs_rows = array();
  $labs_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE approval_status = 1 ORDER BY id DESC");
  while ($labs_data = db_fetch_object($labs_q)) {
  	/* file attachments */
  	$file_links = '';
  	$problem_q = db_query("SELECT * FROM {lab_migration_files} WHERE link_id = %d AND filetype = 'P'", $labs_data->id);
  	$problem_data = db_fetch_object($problem_q);
  	$file_links .= '<br/>' . l('Problem Statement', $dl_root_path . $problem_data->filepath);
  	$sup_q = db_query("SELECT * FROM {lab_migration_files} WHERE link_id = %d AND filetype = 'S'", $labs_data->id);
  	$sup_data = db_fetch_object($sup_q);
  	if ($sup_data)
  		$file_links .= '<br/>' . l('Suplementary Files', $dl_root_path . $sup_data->filepath);

  	$migration_status = '';
  	if ($labs_data->solution_status == 0) {
  		$migration_status = 'In Progress';
  	} else if ($labs_data->solution_status == 1) {
  		$migration_status = 'Migrated';
  		if (($user->uid == $labs_data->uid) || user_access('manage proposal')) {
				$sol_q = db_query("SELECT * FROM {lab_migration_files} WHERE link_id = %d AND filetype = 'A'", $labs_data->id);
				$sol_data = db_fetch_object($sol_q);
  			$file_links .= '<br/>' . l('Solution File', $dl_root_path . $sol_data->filepath);
  		}
  	}

    $labs_rows[] = array($labs_data->university, $labs_data->lab_title, $labs_data->problem_topic . $file_links, l($labs_data->name_title . ' ' . $labs_data->name, 'user/' . $labs_data->uid) . '<br/>' . $labs_data->department, $migration_status);
  }

  /* check if there are any pending proposals */
  if (!$labs_rows) {
    drupal_set_message(t('There are no lab migrations.'), 'status');
    return '';
  }

  $labs_header = array('University/Institute', 'Title of the Lab', 'Title of the Problem', 'Name of the Proposer and Department', 'Status');
  $output = theme_table($labs_header, $labs_rows);
  return $output; 
}
