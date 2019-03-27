<?php
// $Id$
/* function to display list of lab migration solve labs */
function lab_migration_proposal_open()
{
	global $user;
	/* get open proposal list */
	$proposal_rows = array();
	$proposal_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE approval_status = 1 AND solution_provider_uid = 0");
	$num_of_results = $proposal_q->rowCount();
	if($num_of_results != 0)
	{
	while ($proposal_data = $proposal_q->fetchObject())
	{
		$proposal_rows[] = array(
			l($proposal_data->lab_title, 'lab_migration/show_proposal/' . $proposal_data->id),
			l('Apply', 'lab_migration/show_proposal/' . $proposal_data->id)
		);
	} //$proposal_data = $proposal_q->fetchObject()
	$proposal_header = array(
		'Title of the Lab',
		''
	);
	$return_html = theme('table', array(
		'header' => $proposal_header,
		'rows' => $proposal_rows
	));
	} else
	{
	$return_html = "<br><br>No labs are available to solve";
	}//$proposal_q
	//$return_html = theme_table($proposal_header, $proposal_rows);
	return $return_html;
}
function lab_migration_solution_proposal_form($form,&$form_state)
{
	global $user;
	$proposal_id = (int) arg(2);
	//$proposal_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE id = %d", $proposal_id);
	$query = db_select('lab_migration_proposal');
	$query->fields('lab_migration_proposal');
	$query->condition('id', $proposal_id);
	$proposal_q = $query->execute();
	$proposal_data = $proposal_q->fetchObject();
	if (!$proposal_data)
	{
		drupal_set_message("Invalid proposal.", 'error');
		drupal_goto('');
	} //!$proposal_data
	//var_dump($proposal_data->name); die;
	$form['name'] = array(
		'#type' => 'item',
		'#markup' => l($proposal_data->name_title . ' ' . $proposal_data->name, 'user/' . $proposal_data->uid),
		'#title' => t('Proposer Name')
	);
	$form['lab_title'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->lab_title,
		'#title' => t('Title of the Lab')
	);
	$experiment_html = '';
	//$experiment_q = db_query("SELECT * FROM {lab_migration_experiment} WHERE proposal_id = %d", $proposal_id);
	$query = db_select('lab_migration_experiment');
	$query->fields('lab_migration_experiment');
	$query->condition('proposal_id', $proposal_id);
	$experiment_q = $query->execute();
	$i=1;
	while ($experiment_data = $experiment_q->fetchObject())
	{
		$experiment_html .= $i.") ".$experiment_data->title . "<br/>";
		$i++;
	} //$experiment_data = $experiment_q->fetchObject()
	$form['experiment'] = array(
		'#type' => 'item',
		'#markup' => $experiment_html,
		'#title' => t('Experiment List')
	);
	$form['solution_provider_name_title'] = array(
		'#type' => 'select',
		'#title' => t('Title'),
		'#options' => array(
			'Mr' => 'Mr',
			'Ms' => 'Ms',
			'Mrs' => 'Mrs',
			'Dr' => 'Dr',
			'Prof' => 'Prof'
		),
		'#required' => TRUE
	);
	$form['solution_provider_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Name of the Solution Provider'),
		'#size' => 30,
		'#maxlength' => 50,
		'#required' => TRUE,
		'#attributes' => array(
			'placeholder' => 'Insert your full name'
		),
		'#description' => 'Please insert your full name.'
	);
	$form['solution_provider_email_id'] = array(
		'#type' => 'textfield',
		'#title' => t('Email'),
		'#size' => 30,
		'#value' => $user->mail,
		'#disabled' => TRUE
	);
	$form['solution_provider_contact_ph'] = array(
		'#type' => 'textfield',
		'#title' => t('Contact No.'),
		'#size' => 30,
		'#maxlength' => 15,
		'#required' => TRUE
	);
	$form['solution_provider_department'] = array(
		'#type' => 'select',
		'#title' => t('Department/Branch'),
		'#options' => _lm_list_of_departments(),
		'#required' => TRUE
	);
	$form['solution_provider_university'] = array(
		'#type' => 'textfield',
		'#title' => t('University/Institute'),
		'#size' => 50,
		'#maxlength' => 255,
		'#required' => TRUE,
		'#attributes' => array(
			'placeholder' => 'Insert full name of your institute/ university.... '
		),
		'#description' => 'Please insert full name of your institute/ university.'
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Apply for Solution')
	);
	return $form;
}
function lab_migration_solution_proposal_form_validate($form, &$form_state)
{
	global $user;
	//$solution_provider_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE solution_provider_uid = ".$user->uid." AND approval_status IN (0, 1) AND solution_status IN (0, 1, 2)");
	$query = db_select('lab_migration_proposal');
	$query->fields('lab_migration_proposal');
	$query->condition('solution_provider_uid', $user->uid);
	$query->condition('approval_status', array(
		0,
		1
	), 'IN');
	$query->condition('solution_status', array(
		0,
		1,
		2
	), 'IN');
	$solution_provider_q = $query->execute();
	if ($solution_provider_q->fetchObject())
	{
		form_set_error('', t("You have already applied for a solution. Please compelete that before applying for another solution."));
		drupal_goto('lab_migration/open_proposal');
	} //$solution_provider_q->fetchObject()
}
function lab_migration_solution_proposal_form_submit($form, &$form_state)
{
	global $user;
	$proposal_id = (int) arg(2);
	//$proposal_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE id = %d", $proposal_id);
	$query = db_select('lab_migration_proposal');
	$query->fields('lab_migration_proposal');
	$query->condition('id', $proposal_id);
	$proposal_q = $query->execute();
	$proposal_data = $proposal_q->fetchObject();
	if (!$proposal_data)
	{
		drupal_set_message("Invalid proposal.", 'error');
		drupal_goto('lab_migration/open_proposal');
	} //!$proposal_data
	if ($proposal_data->solution_provider_uid != 0)
	{
		drupal_set_message("Someone has already applied for solving this Lab.", 'error');
		drupal_goto('lab_migration/open_proposal');
	} //$proposal_data->solution_provider_uid != 0
	$query = "UPDATE {lab_migration_proposal} set solution_provider_uid = :uid, solution_status = 1, solution_provider_name_title = :solution_provider_name_title, solution_provider_name = :solution_provider_contact_name, solution_provider_contact_ph = :solution_provider_contact_ph, solution_provider_department = :solution_provider_department, solution_provider_university = :solution_provider_university WHERE id = :proposal_id";
	$args = array(
		":uid" => $user->uid,
		":solution_provider_name_title" => $form_state['values']['solution_provider_name_title'],
		":solution_provider_contact_name" => _lm_sentence_case($form_state['values']['solution_provider_name']),
		":solution_provider_contact_ph" => $form_state['values']['solution_provider_contact_ph'],
		":solution_provider_department" => $form_state['values']['solution_provider_department'],
		":solution_provider_university" => _lm_sentence_case($form_state['values']['solution_provider_university']),
		":proposal_id" => $proposal_id
	);
	$result = db_query($query, $args);
	drupal_set_message("We have received your application. We will get back to you soon.", 'status');
	/* sending email */
	$params['solution_proposal_received']['proposal_id'] = $proposal_id;
	$params['solution_proposal_received']['user_id'] = $user->uid;
		$email_to = $user->mail;
		$from = variable_get('lab_migration_from_email', '');
		$bcc = variable_get('lab_migration_bcc_email', '');
		$cc = variable_get('lab_migration_from_email', '');
		$params['solution_proposal_received']['headers'] = array(
			'From' => $from,
			'MIME-Version' => '1.0',
			'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
			'Content-Transfer-Encoding' => '8Bit',
			'X-Mailer' => 'Drupal',
			'Cc' => $cc,
			'Bcc' => $bcc
		);
		if (!drupal_mail('lab_migration', 'solution_proposal_received', $email_to, language_default(), $params, $from, TRUE))
			drupal_set_message('Error sending email message.', 'error');
	drupal_goto('lab_migration/open_proposal');
}
