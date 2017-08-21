<?php
/**
 * @file
 * Functions for managing lab migration proposals.
 */

function lab_migration_proposal_pending() {
  /* get pending proposals to be approved */
  $pending_rows = array();
  //$pending_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE approval_status = 0 ORDER BY id DESC");
  $query = db_select('lab_migration_proposal');
  $query->fields('lab_migration_proposal');
  $query->condition('approval_status', 0);
  $query->orderBy('id', 'DESC');
  $pending_q = $query->execute();
  while ($pending_data = $pending_q->fetchObject()) {
    $pending_rows[$pending_data->id] = array(
      date('d-m-Y', $pending_data->creation_date),
      l($pending_data->name, 'user/' . $pending_data->uid),
      $pending_data->lab_title,
      $pending_data->department,
      l(t('Approve'), 'lab_migration/manage_proposal/approve/' . $pending_data->id) . ' | ' . l(t('Edit'), 'lab_migration/manage_proposal/edit/' . $pending_data->id)
    );
  } //$pending_data = $pending_q->fetchObject()
  /* check if there are any pending proposals */
  if (!$pending_rows) {
    drupal_set_message(t('There are no pending proposals.'), 'status');
    return '';
  } //!$pending_rows
  $pending_header = array(
    'Date of Submission',
    'Name',
    'Title of the Lab',
    'Department',
    'Action'
  );
  //$output = theme_table($pending_header, $pending_rows);
  $output = theme('table', array(
    'header' => $pending_header,
    'rows' => $pending_rows
  ));
  return $output;
}
function lab_migration_proposal_pending_solution() {
  /* get pending proposals to be approved */
  $pending_rows = array();
  //$pending_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE approval_status = 1 ORDER BY id DESC");
  $query = db_select('lab_migration_proposal');
  $query->fields('lab_migration_proposal');
  $query->condition('approval_status', 1);
  $query->orderBy('id', 'DESC');
  $pending_q = $query->execute();
  while ($pending_data = $pending_q->fetchObject()) {
    $pending_rows[$pending_data->id] = array(
      date('d-m-Y', $pending_data->creation_date),
      date('d-m-Y', $pending_data->approval_date),
      l($pending_data->name, 'user/' . $pending_data->uid),
      $pending_data->lab_title,
      $pending_data->department,
      l(t('Status'), 'lab_migration/manage_proposal/status/' . $pending_data->id)
    );
  } //$pending_data = $pending_q->fetchObject()
  /* check if there are any pending proposals */
  if (!$pending_rows) {
    drupal_set_message(t('There are no proposals pending for solutions.'), 'status');
    return '';
  } //!$pending_rows
  $pending_header = array(
    'Date of Submission',
    'Date of Approval',
    'Name',
    'Title of the Lab',
    'Department',
    'Action'
  );
  $output = theme('table', array(
    'header' => $pending_header,
    'rows' => $pending_rows
  ));
  return $output;
}
function lab_migration_proposal_all() {
  /* get pending proposals to be approved */
  $proposal_rows = array();
  //$proposal_q = db_query("SELECT * FROM {lab_migration_proposal} ORDER BY id DESC");
  $query = db_select('lab_migration_proposal');
  $query->fields('lab_migration_proposal');
  $query->orderBy('id', 'DESC');
  $proposal_q = $query->execute();
  while ($proposal_data = $proposal_q->fetchObject()) {
    $approval_status = '';
    switch ($proposal_data->approval_status) {
      case 0:
        $approval_status = 'Pending';
        break;
      case 1:
        $approval_status = 'Approved';
        break;
      case 2:
        $approval_status = 'Dis-approved';
        break;
      case 3:
        $approval_status = 'Solved';
        break;
      default:
        $approval_status = 'Unknown';
        break;
    } //$proposal_data->approval_status
    $proposal_rows[] = array(
      date('d-m-Y', $proposal_data->creation_date),
      l($proposal_data->name, 'user/' . $proposal_data->uid),
      $proposal_data->lab_title,
      $proposal_data->department,
      $approval_status,
      l(t('Status'), 'lab_migration/manage_proposal/status/' . $proposal_data->id) . ' | ' . l(t('Edit'), 'lab_migration/manage_proposal/edit/' . $proposal_data->id)
    );
  } //$proposal_data = $proposal_q->fetchObject()
  /* check if there are any pending proposals */
  if (!$proposal_rows) {
    drupal_set_message(t('There are no proposals.'), 'status');
    return '';
  } //!$proposal_rows
  $proposal_header = array(
    'Date of Submission',
    'Name',
    'Title of the Lab',
    'Department',
    'Status'
  );
  $output = theme('table', array(
    'header' => $proposal_header,
    'rows' => $proposal_rows
  ));
  return $output;
}
function lab_migration_category_all() {
  /* get pending proposals to be approved */
  $proposal_rows = array();
  // $proposal_q = db_query("SELECT * FROM {lab_migration_proposal} ORDER BY id DESC");
  $query = db_select('lab_migration_proposal');
  $query->fields('lab_migration_proposal');
  $query->orderBy('id', 'DESC');
  $proposal_q = $query->execute();
  while ($proposal_data = $proposal_q->fetchObject()) {
    switch ($proposal_data->category) {
      case 0:
        $category_data = 'Not Selected';
        break;
      case 1:
        $category_data = 'Fluid Mechanics';
        break;
      case 2:
        $category_data = 'Control Theory & Control Systems';
        break;
      case 3:
        $category_data = 'Chemical Engineering';
        break;
      case 4:
        $category_data = 'Thermodynamics';
        break;
      case 5:
        $category_data = 'Mechanical Engineering';
        break;
      case 6:
        $category_data = 'Signal Processing';
        break;
      case 7:
        $category_data = 'Digital Communications';
        break;
      case 8:
        $category_data = 'Electrical Technology';
        break;
      case 9:
        $category_data = 'Mathematics & Pure Science';
        break;
      case 10:
        $category_data = 'Analog Electronics';
        break;
      case 11:
        $category_data = 'Digital Electronics';
        break;
      case 12:
        $category_data = 'Computer Programming';
        break;
      case 13:
        $category_data = 'Others';
        break;
      default:
        $category_data = 'Unknown';
        break;
    } //$proposal_data->category
    $proposal_rows[] = array(
      date('d-m-Y', $proposal_data->creation_date),
      l($proposal_data->name, 'user/' . $proposal_data->uid),
      $proposal_data->lab_title,
      $proposal_data->department,
      $category_data,
      l(t('Edit Category'), 'lab_migration/manage_proposal/category/edit/' . $proposal_data->id)
    );
  } //$proposal_data = $proposal_q->fetchObject()
  $proposal_header = array(
    'Date of Submission',
    'Name',
    'Title of the Lab',
    'Department',
    'Category',
    'Action'
  );
  $output = theme('table', array(
    'header' => $proposal_header,
    'rows' => $proposal_rows
  ));
  return $output;
}
/******************************************************************************/
/************************** PROPOSAL APPROVAL FORM ****************************/
/******************************************************************************/
function lab_migration_proposal_approval_form($form_state) {
  global $user;
  /* get current proposal */
  $proposal_id = (int) arg(3);
  //$proposal_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE id = %d", $proposal_id);
  $query = db_select('lab_migration_proposal');
  $query->fields('lab_migration_proposal');
  $query->condition('id', $proposal_id);
  $proposal_q = $query->execute();
  if ($proposal_q) {
    if ($proposal_data = $proposal_q->fetchObject()) {
      /* everything ok */
    } //$proposal_data = $proposal_q->fetchObject()
    else {
      drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
      drupal_goto('lab_migration/manage_proposal');
      return;
    }
  } //$proposal_q
  else {
    drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
    drupal_goto('lab_migration/manage_proposal');
    return;
  }
  // var_dump($proposal_data->name_title);
  //    die;
  $form['name'] = array(
    '#type' => 'item',
    '#markup' => l($proposal_data->name_title . ' ' . $proposal_data->name, 'user/' . $proposal_data->uid),
    '#title' => t('Name')
  );
  $form['email_id'] = array(
    '#type' => 'item',
    '#markup' => user_load($proposal_data->uid)->mail,
    '#title' => t('Email')
  );
  $form['contact_ph'] = array(
    '#type' => 'item',
    '#markup' => $proposal_data->contact_ph,
    '#title' => t('Contact No.')
  );
  $form['department'] = array(
    '#type' => 'item',
    '#markup' => $proposal_data->department,
    '#title' => t('Department/Branch')
  );
  $form['institute'] = array(
    '#type' => 'item',
    '#markup' => $proposal_data->institute,
    '#title' => t('Institute')
  );
  $form['university'] = array(
    '#type' => 'item',
    '#markup' => $proposal_data->university,
    '#title' => t('University')
  );
  $form['lab_title'] = array(
    '#type' => 'item',
    '#markup' => $proposal_data->lab_title,
    '#title' => t('Title of the Lab')
  );
  /* get experiment details */
  $experiment_list = '<ul>';
  //$experiment_q = db_query("SELECT * FROM {lab_migration_experiment} WHERE proposal_id = %d ORDER BY id ASC", $proposal_id);
  $query = db_select('lab_migration_experiment');
  $query->fields('lab_migration_experiment');
  $query->condition('proposal_id', $proposal_id);
  $query->orderBy('id', 'ASC');
  $experiment_q = $query->execute();
  while ($experiment_data = $experiment_q->fetchObject()) {
    $experiment_list .= '<li>' . $experiment_data->title . '</li>';
  } //$experiment_data = $experiment_q->fetchObject()
  $experiment_list .= '</ul>';
  $form['experiment'] = array(
    '#type' => 'item',
    '#markup' => $experiment_list,
    '#title' => t('Experiments')
  );
  if ($proposal_data->solution_provider_uid == 0) {
    $solution_provider = "User will not provide solution, we will have to provide solution";
  } //$proposal_data->solution_provider_uid == 0
  elseif ($proposal_data->solution_provider_uid == $proposal_data->uid) {
    $solution_provider = "Proposer will provide the solution of the lab";
  } //$proposal_data->solution_provider_uid == $proposal_data->uid
  else {
    $solution_provider_user_data = user_load($proposal_data->solution_provider_uid);
    if ($solution_provider_user_data)
      $solution_provider = "Solution will be provided by user " . l($solution_provider_user_data->name, 'user/' . $proposal_data->solution_provider_uid);
    else
      $solution_provider = "User does not exists";
  }
  $form['solution_provider_uid'] = array(
    '#type' => 'item',
    '#title' => t('Do you want to provide the solution'),
    '#markup' => $solution_provider
  );
  $form['solution_display'] = array(
    '#type' => 'item',
    '#title' => t('Do you want to display the solution on the www.scilab.in website'),
    '#markup' => ($proposal_data->solution_display == 1) ? "Yes" : "No"
  );
  $form['approval'] = array(
    '#type' => 'radios',
    '#title' => t('Lab migration proposal'),
    '#options' => array(
      '1' => 'Approve',
      '2' => 'Disapprove'
    ),
    '#required' => TRUE
  );
  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Reason for disapproval')
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit')
  );
  $form['cancel'] = array(
    '#type' => 'item',
    '#markup' => l(t('Cancel'), 'lab_migration/manage_proposal')
  );
  return $form;
}
function lab_migration_proposal_approval_form_submit($form, &$form_state) {
  global $user;
  /* get current proposal */
  $proposal_id = (int) arg(3);
  // $proposal_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE id = %d", $proposal_id);
  $query = db_select('lab_migration_proposal');
  $query->fields('lab_migration_proposal');
  $query->condition('id', $proposal_id);
  $proposal_q = $query->execute();
  if ($proposal_q) {
    if ($proposal_data = $proposal_q->fetchObject()) {
      /* everything ok */
    } //$proposal_data = $proposal_q->fetchObject()
    else {
      drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
      drupal_goto('lab_migration/manage_proposal');
      return;
    }
  } //$proposal_q
  else {
    drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
    drupal_goto('lab_migration/manage_proposal');
    return;
  }
  if ($form_state['values']['approval'] == 1) {
    $query = "UPDATE {lab_migration_proposal} SET approver_uid = :uid, approval_date = :date, approval_status = 1 WHERE id = :proposal_id";
    $args = array(
      ":uid" => $user->uid,
      ":date" => time(),
      ":proposal_id" => $proposal_id
    );
    db_query($query, $args);
    /* sending email */
    $user_data = user_load($proposal_data->uid);
    $email_to = $user_data->mail;
    $param['proposal_approved']['proposal_id'] = $proposal_id;
    $param['proposal_approved']['user_id'] = $proposal_data->uid;
    // echo "To: ".$email_to." From: ".variable_get('lab_migration_from_email', NULL)."<br>";
    // echo "<pre>";
    // var_dump($param); die;
    if (!drupal_mail('lab_migration', 'proposal_approved', $email_to, language_default(), $param, variable_get('lab_migration_from_email', NULL), TRUE))
      drupal_set_message(t('Error sending email message.'), 'error');
    $email_to = $user->mail . ', ' . variable_get('lab_migration_emails', '');
    if (!drupal_mail('lab_migration', 'proposal_approved', $email_to, language_default(), $param, variable_get('lab_migration_from_email', NULL), TRUE))
      drupal_set_message(t('Error sending email message.'), 'error');
    drupal_set_message(t('Lab migration proposal No. @proposal_id approved. User has been notified of the approval.', array('@proposal_id' => $proposal_id)), 'status');
    drupal_goto('lab_migration/manage_proposal');
    return;
  } //$form_state['values']['approval'] == 1
  elseif ($form_state['values']['approval'] == 2) {
    $query = "UPDATE {lab_migration_proposal} SET approver_uid = :uid, approval_date = :date, approval_status = 2, message = :message, solution_provider_uid = 0, solution_status = 0 WHERE id = :proposal_id";
    $args = array(
      ":uid" => $user->uid,
      ":date" => time(),
      ":message" => $form_state['values']['message'],
      ":proposal_id" => $proposal_id
    );
    $result = db_query($query, $args);
    /* sending email */
    $user_data = user_load($proposal_data->uid);
    $email_to = $user_data->mail;
    $param['proposal_disapproved']['proposal_id'] = $proposal_id;
    $param['proposal_disapproved']['user_id'] = $proposal_data->uid;
    if (!drupal_mail('lab_migration', 'proposal_disapproved', $email_to, language_default(), $param, variable_get('lab_migration_from_email', NULL), TRUE))
      drupal_set_message(t('Error sending email message.'), 'error');
    $email_to = $user_data->mail . ', ' . variable_get('lab_migration_emails', '');
    if (!drupal_mail('lab_migration', 'proposal_disapproved', $email_to, language_default(), $param, variable_get('lab_migration_from_email', NULL), TRUE))
      drupal_set_message(t('Error sending email message.'), 'error');
    drupal_set_message(t('Lab migration proposal No. @proposal_id dis-approved. User has been notified of the dis-approval.', array('@proposal_id' => $proposal_id)), 'error');
    drupal_goto('lab_migration/manage_proposal');
    return;
  } //$form_state['values']['approval'] == 2
}
/******************************************************************************/
/*************************** PROPOSAL STATUS FORM *****************************/
/******************************************************************************/
function lab_migration_proposal_status_form($form, $form_state) {
  global $user;
  /* get current proposal */
  $proposal_id = (int) arg(3);
  //$proposal_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE id = %d", $proposal_id);
  $query = db_select('lab_migration_proposal');
  $query->fields('lab_migration_proposal');
  $query->condition('id', $proposal_id);
  $proposal_q = $query->execute();
  if ($proposal_q) {
    if ($proposal_data = $proposal_q->fetchObject()) {
      /* everything ok */
    } //$proposal_data = $proposal_q->fetchObject()
    else {
      drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
      drupal_goto('lab_migration/manage_proposal');
      return;
    }
  } //$proposal_q
  else {
    drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
    drupal_goto('lab_migration/manage_proposal');
    return;
  }
  $form['name'] = array(
    '#type' => 'item',
    '#markup' => l($proposal_data->name_title . ' ' . $proposal_data->name, 'user/' . $proposal_data->uid),
    '#title' => t('Name')
  );
  $form['email_id'] = array(
    '#type' => 'item',
    '#markup' => user_load($proposal_data->uid)->mail,
    '#title' => t('Email')
  );
  $form['contact_ph'] = array(
    '#type' => 'item',
    '#markup' => $proposal_data->contact_ph,
    '#title' => t('Contact No.')
  );
  $form['department'] = array(
    '#type' => 'item',
    '#markup' => $proposal_data->department,
    '#title' => t('Department/Branch')
  );
  $form['institute'] = array(
    '#type' => 'item',
    '#markup' => $proposal_data->institute,
    '#title' => t('Institute')
  );
  $form['university'] = array(
    '#type' => 'item',
    '#markup' => $proposal_data->university,
    '#title' => t('University')
  );
  $form['lab_title'] = array(
    '#type' => 'item',
    '#markup' => $proposal_data->lab_title,
    '#title' => t('Title of the Lab')
  );
  /* get experiment details */
  $experiment_list = '<ul>';
  //$experiment_q = db_query("SELECT * FROM {lab_migration_experiment} WHERE proposal_id = %d ORDER BY id ASC", $proposal_id);
  $query = db_select('lab_migration_experiment');
  $query->fields('lab_migration_experiment');
  $query->condition('proposal_id', $proposal_id);
  $query->orderBy('id', 'ASC');
  $experiment_q = $query->execute();
  while ($experiment_data = $experiment_q->fetchObject()) {
    $experiment_list .= '<li>' . $experiment_data->title . '</li>';
  } //$experiment_data = $experiment_q->fetchObject()
  $experiment_list .= '</ul>';
  $form['experiment'] = array(
    '#type' => 'item',
    '#markup' => $experiment_list,
    '#title' => t('Experiments')
  );
  if ($proposal_data->solution_provider_uid == 0) {
    $solution_provider = "User will not provide solution, we will have to provide solution";
  } //$proposal_data->solution_provider_uid == 0
  elseif ($proposal_data->solution_provider_uid == $proposal_data->uid) {
    $solution_provider = "Proposer will provide the solution of the lab";
  } //$proposal_data->solution_provider_uid == $proposal_data->uid
  else {
    $solution_provider_user_data = user_load($proposal_data->solution_provider_uid);
    if ($solution_provider_user_data)
      $solution_provider = "Solution will be provided by user " . l($solution_provider_user_data->name, 'user/' . $proposal_data->solution_provider_uid);
    else
      $solution_provider = "User does not exists";
  }
  $form['solution_provider_uid'] = array(
    '#type' => 'item',
    '#title' => t('Who will provide the solution'),
    '#markup' => $solution_provider
  );
  $form['solution_display'] = array(
    '#type' => 'item',
    '#title' => t('Display the solution on the www.scilab.in website'),
    '#markup' => ($proposal_data->solution_display == 1) ? "Yes" : "No"
  );
  $proposal_status = '';
  switch ($proposal_data->approval_status) {
    case 0:
      $proposal_status = t('Pending');
      break;
    case 1:
      $proposal_status = t('Approved');
      break;
    case 2:
      $proposal_status = t('Dis-approved');
      break;
    case 3:
      $proposal_status = t('Completed');
      break;
    default:
      $proposal_status = t('Unkown');
      break;
  } //$proposal_data->approval_status
  $form['proposal_status'] = array(
    '#type' => 'item',
    '#markup' => $proposal_status,
    '#title' => t('Proposal Status')
  );
  if ($proposal_data->approval_status == 0) {
    $form['approve'] = array(
      '#type' => 'item',
      '#markup' => l(t('Click here'), 'lab_migration/manage_proposal/approve/' . $proposal_id),
      '#title' => t('Approve')
    );
  } //$proposal_data->approval_status == 0
  if ($proposal_data->approval_status == 2) {
    $form['message'] = array(
      '#type' => 'item',
      '#markup' => $proposal_data->message,
      '#title' => t('Reason for disapproval')
    );
  } //$proposal_data->approval_status == 2
  
  $form['completed'] = array();
  $query = db_query("SELECT * FROM {lab_migration_journal} WHERE proposal_id = (:pid)", array(
    ':pid' => $proposal_data->id
  ));
  // var_dump($proposal_data->id); die;
  if ($query->rowCount() != 0) {
    $val = $query->fetchObject();
    $form['journal'] = array(
      '#type' => 'item',
      '#markup' => l($val->filename, 'lab_migration/download/journal/' . $val->id),
      '#title' => 'Journal File'
    );
  } 
  else {
    $form['journal'] = array(
      '#type' => 'item',
      '#markup' => t("The journal is not uploaded for this lab."),
      '#title' => 'Journal File'
    );
    $form['completed']['#disabled'] = TRUE;
    
  }
  
  $flag = TRUE;
  $exp_query = db_query("SELECT id, number FROM {lab_migration_experiment} WHERE proposal_id = (:pid)", array(
    ':pid' => $proposal_data->id
  ));
  $val = $exp_query->fetchAllKeyed(1, 0);
  //var_dump($val); die;
  foreach ($val as $key => $value) {
    $sol_query = db_query("SELECT * FROM {lab_migration_solution} WHERE experiment_id = (:eid)", array(
      ':eid' => $value
    ));
    if ($sol_query->rowCount() == 0) {
      $flag = FALSE;
      $form['exp_status'] = array(
        '#type' => 'item',
        '#markup' => "Solution for Experiment $key of this lab not submitted."
      );
      break;
    } 
    elseif ($sol_query->fetchObject()->approval_status == 0) {
      $flag = FALSE;
      $form['exp_status'] = array(
        '#type' => 'item',
        '#markup' => "Solution for Experiment $key of this lab not approved."
      );
      break;
    }
  }
  
  if (!$flag)
    $form['completed']['#disabled'] = TRUE;
  
  if ($proposal_data->approval_status == 1) {
    $form['completed']['#type'] = 'checkbox';
    $form['completed']['#title'] = t('Completed');
    $form['completed']['#description'] = t('Check if user has provided all experiment solution and uploaded journal.');
    $form['completed']['#weight'] = 1;
  } //$proposal_data->approval_status == 1
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#weight' => 2
  );
  $form['cancel'] = array(
    '#type' => 'item',
    '#markup' => l(t('Cancel'), 'lab_migration/manage_proposal/all'),
    '#weight' => 3
  );
  return $form;
}
function lab_migration_proposal_status_form_submit($form, &$form_state) {
  global $user;
  /* get current proposal */
  $proposal_id = (int) arg(3);
  //$proposal_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE id = %d", $proposal_id);
  $query = db_select('lab_migration_proposal');
  $query->fields('lab_migration_proposal');
  $query->condition('id', $proposal_id);
  $proposal_q = $query->execute();
  if ($proposal_q) {
    if ($proposal_data = $proposal_q->fetchObject()) {
      /* everything ok */
    } //$proposal_data = $proposal_q->fetchObject()
    else {
      drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
      drupal_goto('lab_migration/manage_proposal');
      return;
    }
  } //$proposal_q
  else {
    drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
    drupal_goto('lab_migration/manage_proposal');
    return;
  }
  /* set the book status to completed */
  if ($form_state['values']['completed'] == 1) {
    $result = db_update('lab_migration_proposal')->fields(array(
      'approval_status' => 3,
      'solution_date' => time()
    ))->condition('id', $proposal_id)->execute();
    /* sending email */
    $user_data = user_load($proposal_data->uid);
    $email_to = $user_data->mail;
    $param['proposal_completed']['proposal_id'] = $proposal_id;
    $param['proposal_completed']['user_id'] = $proposal_data->uid;
    if (!drupal_mail('lab_migration', 'proposal_completed', $email_to, language_default(), $param, variable_get('lab_migration_from_email', NULL), TRUE))
      drupal_set_message(t('Error sending email message.'), 'error');
    $email_to = $user->mail . ', ' . variable_get('lab_migration_emails', '');
    if (!drupal_mail('lab_migration', 'proposal_completed', $email_to, language_default(), $param, variable_get('lab_migration_from_email', NULL), TRUE))
      drupal_set_message(t('Error sending email message.'), 'error');
    drupal_set_message(t('Congratulations! Lab Migration proposal has been marked as completed. User has been notified of the completion.'), 'status');
  } //$form_state['values']['completed'] == 1
  drupal_goto('lab_migration/manage_proposal');
  return;
}
/******************************************************************************/
/**************************** PROPOSAL EDIT FORM ******************************/
/******************************************************************************/
function lab_migration_proposal_edit_form($form, $form_state) {
  global $user;
  /* get current proposal */
  $proposal_id = (int) arg(3);
  //$proposal_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE id = %d", $proposal_id);
  $query = db_select('lab_migration_proposal');
  $query->fields('lab_migration_proposal');
  $query->condition('id', $proposal_id);
  $proposal_q = $query->execute();
  if ($proposal_q) {
    if ($proposal_data = $proposal_q->fetchObject()) {
      /* everything ok */
    } //$proposal_data = $proposal_q->fetchObject()
    else {
      drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
      drupal_goto('lab_migration/manage_proposal');
      return;
    }
  } //$proposal_q
  else {
    drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
    drupal_goto('lab_migration/manage_proposal');
    return;
  }
  $user_data = user_load($proposal_data->uid);
  $form['name_title'] = array(
    '#type' => 'select',
    '#title' => t('Title'),
    '#options' => array(
      'Mr' => 'Mr',
      'Ms' => 'Ms',
      'Mrs' => 'Mrs',
      'Dr' => 'Dr',
      'Prof' => 'Prof'
    ),
    '#required' => TRUE,
    '#default_value' => $proposal_data->name_title
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name of the Proposer'),
    '#size' => 30,
    '#maxlength' => 50,
    '#required' => TRUE,
    '#default_value' => $proposal_data->name
  );
  $form['email_id'] = array(
    '#type' => 'item',
    '#title' => t('Email'),
    '#markup' => $user_data->mail
  );
  $form['contact_ph'] = array(
    '#type' => 'textfield',
    '#title' => t('Contact No.'),
    '#size' => 30,
    '#maxlength' => 15,
    '#required' => TRUE,
    '#default_value' => $proposal_data->contact_ph
  );
  $form['department'] = array(
    '#type' => 'select',
    '#title' => t('Department/Branch'),
    '#options' => array(
      '' => 'Please select...',
      'Computer Engineering' => 'Computer Engineering',
      'Electrical Engineering' => 'Electrical Engineering',
      'Electronics Engineering' => 'Electronics Engineering',
      'Chemical Engineering' => 'Chemical Engineering',
      'Instrumentation Engineering' => 'Instrumentation Engineering',
      'Mechanical Engineering' => 'Mechanical Engineering',
      'Civil Engineering' => 'Civil Engineering',
      'Physics' => 'Physics',
      'Mathematics' => 'Mathematics',
      'Others' => 'Others'
    ),
    '#required' => TRUE,
    '#default_value' => $proposal_data->department
  );
  $form['institute'] = array(
    '#type' => 'textfield',
    '#title' => t('Institute'),
    '#size' => 50,
    '#maxlength' => 200,
    '#required' => TRUE,
    '#default_value' => $proposal_data->institute,
  );
  $form['university'] = array(
    '#type' => 'textfield',
    '#title' => t('University'),
    '#size' => 50,
    '#maxlength' => 200,
    '#required' => TRUE,
    '#default_value' => $proposal_data->university,
  );
  $form['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City/ Village'),
    '#size' => 80,
    '#maxlength' => 150,
    '#default_value' => $proposal_data->city,
    '#required' => TRUE,
    '#attributes' => array(
      'placeholder' => 'Insert location of your institute/ university....'
    )
  );
  $form['pincode'] = array(
    '#type' => 'textfield',
    '#title' => t('Pincode'),
    '#size' => 30,
    '#maxlength' => 6,
    '#required' => TRUE,
    '#default_value' => $proposal_data->pincode,
    '#attributes' => array(
      'placeholder' => 'Insert pincode of your city/ village....'
    )
  );
  $form['all_state'] = array(
    '#type' => 'select',
    '#title' => t('State'),
    '#options' => array(
      '0' => 'Please select...',
      'Andhra Pradesh' => t('Andhra Pradesh'),
      'Arunachal Pradesh' => 'Arunachal Pradesh',
      'Assam' => 'Assam',
      'Bihar' => 'Bihar',
      'Chhattisgarh' => 'Chhattisgarh',
      'Goa' => 'Goa',
      'Gujarat' => 'Gujarat',
      'Haryana' => 'Haryana',
      'Himachal Pradesh' => 'Himachal Pradesh',
      'Jammu & Kashmir' => 'Jammu & Kashmir',
      'Jharkhand' => 'Jharkhand',
      'Karnataka' => 'Karnataka',
      'Kerala' => 'Kerala',
      'Madhya Pradesh' => 'Madhya Pradesh',
      'Maharashtra' => 'Maharashtra',
      'Manipur' => 'Manipur',
      'Meghalaya' => 'Meghalaya',
      'Mizoram' => 'Mizoram',
      'Nagaland' => 'Nagaland',
      'Odisha' => 'Odisha',
      'Punjab' => 'Punjab',
      'Rajasthan' => 'Rajasthan',
      'Sikkim' => 'Sikkim',
      'Tamil Nadu' => 'Tamil Nadu',
      'Telangana' => 'Telangana',
      'Tripura' => 'Tripura',
      'Uttarakhand' => 'Uttarakhand',
      'Uttar Pradesh' => 'Uttar Pradesh',
      'West Bengal' => 'West Bengal',
      'Andaman & Nicobar' => 'Andaman & Nicobar',
      'Chandigarh' => 'Chandigarh',
      'Dadra and Nagar Haveli' => 'Dadra and Nagar Haveli',
      'Daman & Diu' => 'Daman & Diu',
      'Delhi' => 'Delhi',
      'Lakshadweep' => 'Lakshadweep',
      'Puducherry' => 'Puducherry',
      'Others' => 'Others'
    ),
    '#required' => TRUE,
    '#tree' => TRUE,
    '#validated' => TRUE,
    '#default_value' => $proposal_data->state
  );
  $form['lab_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title of the Lab'),
    '#size' => 30,
    '#maxlength' => 50,
    '#required' => TRUE,
    '#default_value' => $proposal_data->lab_title
  );
  /* get experiment details */
  // $experiment_q = db_query("SELECT * FROM {lab_migration_experiment} WHERE proposal_id = %d ORDER BY id ASC", $proposal_id);
  $query = db_select('lab_migration_experiment');
  $query->fields('lab_migration_experiment');
  $query->condition('proposal_id', $proposal_id);
  $query->orderBy('id', 'ASC');
  $experiment_q = $query->execute();
  $form['lab_experiment'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#tree' => TRUE
  );
  for ($counter = 1; $counter <= 15; $counter++) {
    $experiment_title = '';
    $experiment_data = $experiment_q->fetchObject();
    if ($experiment_data) {
      $experiment_title = $experiment_data->title;
      $form['lab_experiment']['update'][$experiment_data->id] = array(
        '#type' => 'textfield',
        '#title' => t('Title of the Experiment @counter', array('@counter' => $counter)),
        '#size' => 50,
        '#required' => FALSE,
        '#tree' => TRUE,
        '#default_value' => $experiment_title
      );
    } //$experiment_data
    else {
      $form['lab_experiment']['insert'][$counter] = array(
        '#type' => 'textfield',
        '#title' => t('Title of the Experiment @counter', array('@counter' => $counter)),
        '#size' => 50,
        '#required' => FALSE,
        '#default_value' => $experiment_title
      );
    }
  } //$counter = 1; $counter <= 15; $counter++
  if ($proposal_data->solution_provider_uid == 0) {
    $solution_provider_user = 'Open';
  } //$proposal_data->solution_provider_uid == 0
  elseif ($proposal_data->solution_provider_uid == $proposal_data->uid) {
    $solution_provider_user = 'Proposer';
  } //$proposal_data->solution_provider_uid == $proposal_data->uid
  else {
    $user_data = user_load($proposal_data->solution_provider_uid);
    if (!$user_data) {
      $solution_provider_user = 1;
      drupal_set_message(t('Solution provider user name is invalid'), 'error');
    } //!$user_data
    $solution_provider_user = $user_data->name;
  }
  $form['solution_provider_uid'] = array(
    '#type' => 'item',
    '#title' => t('Who will provide the solution'),
    '#markup' => $solution_provider_user
  );
  $form['open_solution'] = array(
    '#type' => 'checkbox',
    '#title' => t('Open the solution for everyone')
  );
  // $form['solution_display'] = array(
  // 	'#type' => 'radios',
  // 	'#title' => t('Do you want to display the solution on the www.scilab.in website'),
  // 	'#options' => array(
  // 		'1' => 'Yes',
  // 		'2' => 'No'
  // 	),
  // 	'#required' => TRUE,
  // 	'#default_value' => ($proposal_data->solution_display == 1) ? "1" : "2"
  // );
  $form['delete_proposal'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete Proposal')
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit')
  );
  $form['cancel'] = array(
    '#type' => 'item',
    '#markup' => l(t('Cancel'), 'lab_migration/manage_proposal')
  );
  return $form;
}
function lab_migration_proposal_edit_form_validate($form, &$form_state) {
  $proposal_id = (int) arg(3);
  /* check before delete proposal */
  if ($form_state['values']['delete_proposal'] == 1) {
    //$experiment_q = db_query("SELECT * FROM {lab_migration_experiment} WHERE proposal_id = %d", $proposal_id);
    $query = db_select('lab_migration_experiment');
    $query->fields('lab_migration_experiment');
    $query->condition('proposal_id', $proposal_id);
    $experiment_q = $query->execute();
    while ($experiment_data = $experiment_q->fetchObject()) {
      //$solution_q = db_query("SELECT * FROM {lab_migration_solution} WHERE experiment_id = %d", $experiment_data->id);
      $query = db_select('lab_migration_solution');
      $query->fields('lab_migration_solution');
      $query->condition('experiment_id', $experiment_data->id);
      $solution_q = $query->execute();
      if ($solution_q->fetchObject()) {
        form_set_error('', t('Cannot delete proposal since there are solutions already uploaded. Use the "Bulk Manage" interface to delete this proposal'));
      } //$solution_q->fetchObject()
    } //$experiment_data = $experiment_q->fetchObject()
  } //$form_state['values']['delete_proposal'] == 1
  return;
}
function lab_migration_proposal_edit_form_submit($form, &$form_state) {
  global $user;
  /* get current proposal */
  $proposal_id = (int) arg(3);
  // $proposal_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE id = %d", $proposal_id);
  $query = db_select('lab_migration_proposal');
  $query->fields('lab_migration_proposal');
  $query->condition('id', $proposal_id);
  $proposal_q = $query->execute();
  if ($proposal_q) {
    if ($proposal_data = $proposal_q->fetchObject()) {
      /* everything ok */
    } //$proposal_data = $proposal_q->fetchObject()
    else {
      drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
      drupal_goto('lab_migration/manage_proposal');
      return;
    }
  } //$proposal_q
  else {
    drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
    drupal_goto('lab_migration/manage_proposal');
    return;
  }
  /* delete proposal */
  if ($form_state['values']['delete_proposal'] == 1) {
    //db_query("DELETE FROM {lab_migration_proposal} WHERE id = %d", $proposal_id);
    $query = db_delete('lab_migration_proposal');
    $query->condition('id', $proposal_id);
    $num_deleted = $query->execute();
    //db_query("DELETE FROM {lab_migration_experiment} WHERE proposal_id = %d", $proposal_id);
    $query = db_delete('lab_migration_experiment');
    $query->condition('proposal_id', $proposal_id);
    $num_deleted = $query->execute();
    drupal_set_message(t('Proposal Delete'), 'status');
    drupal_goto('lab_migration/manage_proposal');
    return;
  } //$form_state['values']['delete_proposal'] == 1
  if ($form_state['values']['open_solution'] == 1) {
    // $query = "UPDATE {lab_migration_proposal} SET solution_provider_uid = :solution_provider_uid, solution_status = :solution_status, solution_provider_name_title = '', solution_provider_name = '', solution_provider_contact_ph = '', solution_provider_department = '', solution_provider_university = '' WHERE id = :proposal_id";
    // $args= array(
    //    ":solution_provider_uid" => 0, 
    //    ":solution_status" => 0,
    //    ":proposal_id" => $proposal_id,
    // );
    // $result = db_query($query, $args);
    $result = db_update('lab_migration_proposal')->fields(array(
      'solution_provider_uid' => 0,
      'solution_status' => 0,
      'solution_provider_name_title' => '',
      'solution_provider_name' => '',
      'solution_provider_contact_ph' => '',
      'solution_provider_department' => '',
      'solution_provider_university' => ''
    ))->condition('id', $proposal_id)->execute();
    if (!$result) {
      drupal_set_message(t('Solution already open for everyone.'), 'error');
      return;
    } //!$result
  } //$form_state['values']['open_solution'] == 1
  $solution_display = 0;
  if ($form_state['values']['solution_display'] == 1) {
    $solution_display = 1;
  } //$form_state['values']['solution_display'] == 1
  else {
    $solution_display = 0;
  }
  /* update proposal */
  $v = $form_state['values'];
  //$query = "UPDATE {lab_migration_proposal} SET name_title = :name_title, name = :name, contact_ph = :contact_ph, department = :department, university = :unversity, lab_title = :lab_title, solution_display = :solution_display WHERE id = :id";
  // $args= array(    
  //    ":name_title" => $v['name_title'],
  //    ":name" => $v['name'],
  //    "contact_ph" => $v['contact_ph'],
  //    ":department" => $v['department'],
  //    ":university" => $v['university'],
  //    ":lab_title" => $v['lab_title'],
  //    ":solution_display" => $solution_display,
  //    ":id" => $proposal_id,
  //  );
  $query = db_update('lab_migration_proposal')->fields(array(
    'name_title' => $v['name_title'],
    'name' => $v['name'],
    'contact_ph' => $v['contact_ph'],
    'department' => $v['department'],
    'institute' => $v['institute'],
    'university' => $v['university'],
    'city' => $v['city'],
    'pincode' => $v['pincode'],
    'state' => $v['all_state'],
    'lab_title' => $v['lab_title'],
    'solution_display' => $solution_display
  ))->condition('id', $proposal_id);
  $result1 = $query->execute();
  //$result=db_query($query, $args);
  /* updating existing experiments */
  foreach ($form_state['values']['lab_experiment']['update'] as $update_id => $update_value) {
    if (strlen(trim($update_value)) >= 1) {
      $query = "UPDATE {lab_migration_experiment} SET title = :title WHERE id = :id";
      $args = array(
        ":title" => trim($update_value),
        ":id" => $update_id
      );
      $result2 = db_query($query, $args);
      if (!$result2) {
        drupal_set_message(t('Could not update Title of the Experiment : @update_value', array('@update_value' => trim($update_value))), 'error');
      } //!$result2
    } //strlen(trim($update_value)) >= 1
    else {
      $query = "DELETE FROM {lab_migration_experiment} WHERE id = :id LIMIT 1";
      $args = array(
        ":id" => $update_id
      );
      $result3 = db_query($query, $args);
    }
  } //$form_state['values']['lab_experiment']['update'] as $update_id => $update_value
  /* inserting new experiments */
  $query = db_select('lab_migration_experiment');
  $query->fields('lab_migration_experiment');
  $query->condition('proposal_id', $proposal_id);
  $query->orderBy('number', 'DESC');
  $query->range(0, 1);
  $number_q = $query->execute();
  if ($number_data = $number_q->fetchObject()) {
    $number = (int) $number_data->number;
    $number++;
  } //$number_data = $number_q->fetchObject()
  else {
    $number = 1;
  }
  $insertvalue = array(
    $insert_id => $insert_value
  );
  $lab_experimentinsert = $form_state['values']['lab_experiment']['insert'];
  if (is_array($lab_experimentinsert) || is_object($lab_experimentinsert)) {
    foreach ($lab_experimentinsert as $insertvalue) {
      //foreach ($form_state['values']['lab_experiment']['insert'] as $insert_id => $insert_value) {
      if (strlen(trim($insert_value)) >= 1) {
        $query = "INSERT INTO {lab_migration_experiment} (proposal_id, number, title) VALUES :proposal_id, :number, :insert_value";
        $args = array(
          ":proposal_id" => $proposal_id,
          ":number" => $number,
          ":title" => trim($insert_value)
        );
        $result4 = db_query($query, $args);
        if (!$result4) {
          drupal_set_message(t('Could not insert Title of the Experiment : @insert_value', array('@insert_value' => trim($insert_value))), 'error');
        } //!$result4
        else {
          $number++;
        }
      } //strlen(trim($insert_value)) >= 1
    } //$lab_experimentinsert as $insertvalue
  } //is_array($lab_experimentinsert) || is_object($lab_experimentinsert)
  drupal_set_message(t('Proposal Updated'), 'status');
}
/******************************************************************************/
/**************************** CATEGORY EDIT FORM ******************************/
/******************************************************************************/
function lab_migration_category_edit_form($form, &$form_state) {
  /* get current proposal */
  $proposal_id = (int) arg(4);
  //$proposal_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE id = %d", $proposal_id);
  $query = db_select('lab_migration_proposal');
  $query->fields('lab_migration_proposal');
  $query->condition('id', $proposal_id);
  $proposal_q = $query->execute();
  if ($proposal_q) {
    if ($proposal_data = $proposal_q->fetchObject()) {
      /* everything ok */
    } //$proposal_data = $proposal_q->fetchObject()
    else {
      drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
      drupal_goto('lab_migration/manage_proposal');
      return;
    }
  } //$proposal_q
  else {
    drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
    drupal_goto('lab_migration/manage_proposal');
    return;
  }
  $form['name'] = array(
    '#type' => 'item',
    '#markup' => l($proposal_data->name_title . ' ' . $proposal_data->name, 'user/' . $proposal_data->uid),
    '#title' => t('Name')
  );
  $form['email_id'] = array(
    '#type' => 'item',
    '#markup' => user_load($proposal_data->uid)->mail,
    '#title' => t('Email')
  );
  $form['contact_ph'] = array(
    '#type' => 'item',
    '#markup' => $proposal_data->contact_ph,
    '#title' => t('Contact No.')
  );
  $form['department'] = array(
    '#type' => 'item',
    '#markup' => $proposal_data->department,
    '#title' => t('Department/Branch')
  );
  $form['institute'] = array(
    '#type' => 'item',
    '#markup' => $proposal_data->institute,
    '#title' => t('Institute')
  );
  $form['university'] = array(
    '#type' => 'item',
    '#markup' => $proposal_data->university,
    '#title' => t('University')
  );
  $form['lab_title'] = array(
    '#type' => 'item',
    '#markup' => $proposal_data->lab_title,
    '#title' => t('Title of the Lab')
  );
  $form['category'] = array(
    '#type' => 'select',
    '#title' => t('Category'),
    '#options' => _lm_list_of_category(),
    '#required' => TRUE,
    '#default_value' => $proposal_data->category
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit')
  );
  $form['cancel'] = array(
    '#type' => 'item',
    '#markup' => l(t('Cancel'), 'lab_migration/manage_proposal/category')
  );
  return $form;
}
function lab_migration_category_edit_form_submit($form, &$form_state) {
  /* get current proposal */
  $proposal_id = (int) arg(4);
  //$proposal_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE id = %d", $proposal_id);
  $query = db_select('lab_migration_proposal');
  $query->fields('lab_migration_proposal');
  $query->condition('id', $proposal_id);
  $proposal_q = $query->execute();
  if ($proposal_q) {
    if ($proposal_data = $proposal_q->fetchObject()) {
      /* everything ok */
    } //$proposal_data = $proposal_q->fetchObject()
    else {
      drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
      drupal_goto('lab_migration/manage_proposal');
      return;
    }
  } //$proposal_q
  else {
    drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
    drupal_goto('lab_migration/manage_proposal');
    return;
  }
  $query = "UPDATE {lab_migration_proposal} SET category = category WHERE id = :proposal_id";
  $args = array(
    ":category" => $form_state['values']['category'],
    ":proposal_id" => $proposal_data->id
  );
  $result = db_query($query, $args);
  drupal_set_message(t('Proposal Category Updated'), 'status');
  drupal_goto('lab_migration/manage_proposal/category');
}
