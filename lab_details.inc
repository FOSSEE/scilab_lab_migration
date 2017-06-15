<?php
/**
 * @file
 * Contains functions to list labs under progress and completed labs.
 */

/* function to display cpmleted labs */
function lab_migration_completed_labs_all() {
  $page_content = "";
  $query = "SELECT * FROM {lab_migration_proposal} WHERE approval_status = 3 ORDER BY approval_date ASC";
  $result = db_query($query);
  $proposal_rows = array();
  $i = 1;
  while ($row = $result->fetchObject()) {
    $approval_date = date("Y", $row->approval_date);
    $preference_rows[] = array(
      $i,
      $row->university,
      l($row->lab_title, "lab_migration_run/" . $row->id),
      $approval_date
    );
    $i++;
  } //$row = $result->fetchObject()
  $preference_header = array(
    'No',
    'Institute',
    'Lab',
    'Year'
  );
  $output = theme('table', array(
    'header' => $preference_header,
    'rows' => $preference_rows
  ));
  return $output;
}
/* function to display labs in progress */
function lab_migration_labs_progress_all() {
  $page_content = "";
  $query = "SELECT * FROM {lab_migration_proposal} WHERE approval_status = 1 and solution_status = 2 ORDER BY approval_date ASC";
  $result = db_query($query);
  $proposal_rows = array();
  $i = 1;
  while ($row = $result->fetchObject()) {
    //$approval_date = date("Y", $row->approval_date); // remove comment to display year
    $preference_rows[] = array(
      $i,
      $row->university,
      $row->lab_title
      //$approval_date // remove comment to display year
    );
    $i++;
  } //$row = $result->fetchObject()
  $preference_header = array(
    'No',
    'Institute',
    'Lab'
    //'Year' // remove comment to display year
  );
  $output = theme('table', array(
    'header' => $preference_header,
    'rows' => $preference_rows
  ));
  return $output;
}
