<?php
// $Id$
/*
Approval Status :
0 - Pending
1 - Approved
2 - Dis-Approved
3 - Solved
Solution Status :
0 - Pending
1 - Approved
2 - Dis-Approved
Solution Display :
0 - No
1 - Yes

Tables :
lab_migration_solution : approval_status
0 - Pending
1 - Approved
2 - Disapproved (delete it)
*/
/* lab migration proposal form */
function lab_migration_proposal_form($form, &$form_state)
{
	global $user;
	/************************ start approve book details ************************/
	$query = db_select('lab_migration_proposal');
	$query->fields('lab_migration_proposal');
	$query->condition('uid', $user->uid);
	$query->orderBy('id', 'DESC');
	$query->range(0, 1);
	$proposal_q = $query->execute();
	$proposal_data = $proposal_q->fetchObject();
	if ($proposal_data)
	{
		if ($proposal_data->approval_status == 0 || $proposal_data->approval_status == 1)
		{
			drupal_set_message(t('We have already received your proposal.'), 'status');
			drupal_goto('');
			return;
		} //$proposal_data->approval_status == 0 || $proposal_data->approval_status == 1
	} //$proposal_data
	$form['#attributes'] = array(
		'enctype' => "multipart/form-data"
	);
	$form['name_title'] = array(
		'#type' => 'select',
		'#title' => t('Title'),
		'#options' => array(
			'Dr' => 'Dr',
			'Prof' => 'Prof'
		),
		'#required' => TRUE
	);
	$form['name'] = array(
		'#type' => 'textfield',
		'#title' => t('Name of the Proposer'),
		'#size' => 30,
		'#maxlength' => 50,
		'#required' => TRUE
	);
	$form['email_id'] = array(
		'#type' => 'textfield',
		'#title' => t('Email'),
		'#size' => 30,
		'#value' => $user->mail,
		'#disabled' => TRUE
	);
	$form['contact_ph'] = array(
		'#type' => 'textfield',
		'#title' => t('Contact No.'),
		'#size' => 30,
		'#maxlength' => 15,
		'#required' => TRUE
	);
	$form['department'] = array(
		'#type' => 'select',
		'#title' => t('Department/Branch'),
		'#options' => _lm_list_of_departments(),
		'#required' => TRUE
	);
	$form['university'] = array(
		'#type' => 'textfield',
		'#title' => t('University/ Institute'),
		'#size' => 80,
		'#maxlength' => 200,
		'#required' => TRUE,
		'#attributes' => array(
			'placeholder' => 'Insert full name of your institute/ university.... '
		),
		'#description' => 'Please insert full name of your institute/ university.'
	);
	$form['city'] = array(
		'#type' => 'select',
		'#title' => t('City/ Village'),
		'#options' => _lm_list_of_cities(),
		'#maxlength' => 150,
		'#required' => TRUE,
		'#description' => 'Please select location of your institute/ university.'
	);
	$form['pincode'] = array(
		'#type' => 'textfield',
		'#title' => t('Pincode'),
		'#size' => 30,
		'#maxlength' => 6,
		'#required' => TRUE,
		'#attributes' => array(
			'placeholder' => 'Insert pincode of your city/ village....'
		),
		'#description' => 'Please insert pincode of your institute/ university.'
	);
	$form['all_state'] = array(
		'#type' => 'select',
		'#title' => t('State'),
		'#options' => _lm_list_of_states(),
		'#required' => TRUE,
		'#tree' => TRUE,
		'#validated' => TRUE,
		'#description' => 'Please select state of your institute/ university.'
	);
	$form['lab_title'] = array(
		'#type' => 'textfield',
		'#title' => t('Title of the Lab'),
		'#size' => 50,
		'#required' => TRUE
	);
	$first_experiemnt = TRUE;
	for ($counter = 1; $counter <= 15; $counter++)
	{
		if($counter<= variable_get('lab_migration_minimum_exp_limit', '')){
			$form['lab_experiment-' . $counter] = array(
				'#type' => 'textfield',
				'#title' => t('Title of the Experiment ') . $counter,
				'#size' => 50,
				'#required' => TRUE
			);
		}else
		{
			$form['lab_experiment-' . $counter] = array(
					'#type' => 'textfield',
					'#title' => t('Title of the Experiment ') . $counter,
					'#size' => 50,
					'#required' => False
				);
		}
		$first_experiemnt = FALSE;
	} //$counter = 1; $counter <= 15; $counter++
	$form['solution_provider_uid'] = array(
		'#type' => 'radios',
		'#title' => t('Do you want to provide the solution'),
		'#options' => array(
			'1' => 'Yes',
			'2' => 'No'
		),
		'#required' => TRUE,
		'#default_value' => '2',
		'#description' => 'If you dont want to provide the solution then it will be opened for the community. Anyone may come forward and provide the solution.'
	);
	/*$form['solution_display'] = array(
		'#type' => 'radios',
		'#title' => t('Do you want to display the solution on the www.scilab.in website'),
		'#options' => array(
			'1' => 'Yes',
			'2' => 'No'
		),
		'#required' => TRUE,
		'#default_value' => '1',
		'#description' => 'If yes, solutions will be made available to everyone for downloading.',
		'#disabled' => FALSE
	);*/
	/*$form['solution_display'] = array(
	'#type' => 'hidden',
	//  '#title' => t('Do you want to display the solution on the www.scilab.in website'),
	// '#options' => array('1' => 'Yes', '2' => 'No'),
	// '#required' => TRUE,
	'#default_value' => '1'
	//  '#description' => 'If yes, solutions will be made available to everyone for downloading.',
	//   '#disabled' => FALSE,
	);*/
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	return $form;
}
function lab_migration_proposal_form_validate($form, &$form_state)
{
	if (!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['contact_ph']))
	{
		form_set_error('contact_ph', t('Invalid contact phone number'));
	} //!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['contact_ph'])
	else
	{
		return;
	}
}
function lab_migration_proposal_form_submit($form, &$form_state)
{
	global $user;
	if (!$user->uid)
	{
		drupal_set_message('It is mandatory to login on this website to access the proposal form', 'error');
		return;
	} //!$user->uid
	$form_state['values']['name'] = _lm_sentence_case($form_state['values']['name']);
	$form_state['values']['lab_title'] = _lm_sentence_case($form_state['values']['lab_title']);
	$form_state['values']['university'] = _lm_sentence_case($form_state['values']['university']);
	$solution_provider_uid = 0;
	$solution_status = 0;
	$solution_provider_name_title = '';
	$solution_provider_name = '';
	$solution_provider_contact_ph = '';
	$solution_provider_department = '';
	$solution_provider_university = '';
	if ($form_state['values']['solution_provider_uid'] == "1")
	{
		$solution_provider_uid = $user->uid;
		$solution_status = 2;
		$solution_provider_name_title = $form_state['values']['name_title'];
		$solution_provider_name = _lm_sentence_case($form_state['values']['name']);
		$solution_provider_contact_ph = $form_state['values']['contact_ph'];
		$solution_provider_department = $form_state['values']['department'];
		$solution_provider_university = _lm_sentence_case($form_state['values']['university']);
	} //$form_state['values']['solution_provider_uid'] == "1"
	else
	{
		$solution_provider_uid = 0;
	}
	$solution_display = 1;
	/*if ($form_state['values']['solution_display'] == "1")
		$solution_display = 1;
	else
		$solution_display = 0;*/
	/* inserting the user proposal */
	$v = $form_state["values"];
	$result = "INSERT INTO {lab_migration_proposal} 
    (uid, approver_uid, name_title, name, contact_ph, department, university, city, pincode, state, lab_title, approval_status, solution_status, solution_provider_uid, solution_display, creation_date, approval_date, solution_date, solution_provider_name_title, solution_provider_name, solution_provider_contact_ph, solution_provider_department, solution_provider_university) VALUES
    (:uid, :approver_uid, :name_title, :name, :contact_ph, :department, :university, :city, :pincode, :state, :lab_title, :approval_status, :solution_status, :solution_provider_uid, :solution_display, :creation_date, :approval_date, :solution_date, :solution_provider_name_title, :solution_provider_name, :solution_provider_contact_ph, :solution_provider_department, :solution_provider_university)";
	$args = array(
		":uid" => $user->uid,
		":approver_uid" => 0,
		":name_title" => $v['name_title'],
		":name" => $v['name'],
		":contact_ph" => $v['contact_ph'],
		":department" => $v['department'],
		":university" => $v['university'],
		":city" => $v['city'],
		":pincode" => $v['pincode'],
		":state" => $v['all_state'],
		":lab_title" => $v['lab_title'],
		":approval_status" => 0,
		":solution_status" => $solution_status,
		":solution_provider_uid" => $solution_provider_uid,
		":solution_display" => $solution_display,
		":creation_date" => time(),
		":approval_date" => 0,
		":solution_date" => 0,
		":solution_provider_name_title" => $solution_provider_name_title,
		":solution_provider_name" => $solution_provider_name,
		":solution_provider_contact_ph" => $solution_provider_contact_ph,
		":solution_provider_department" => $solution_provider_department,
		":solution_provider_university" => $solution_provider_university
	);
	$proposal_id = db_query($result, $args, array(
		'return' => Database::RETURN_INSERT_ID
	));
	if (!$proposal_id)
	{
		drupal_set_message(t('Error receiving your proposal. Please try again.'), 'error');
		return;
	} //!$proposal_id
	/* proposal id */
	//$proposal_id = db_last_insert_id('lab_migration_proposal', 'id');
	/* adding experiments */
	$number = 1;
	for ($counter = 1; $counter <= 15; $counter++)
	{
		$experiment_field_name = 'lab_experiment-' . $counter;
		if (strlen(trim($form_state['values'][$experiment_field_name])) >= 1)
		{
			$form_state['values'][$experiment_field_name] = _lm_sentence_case($form_state['values'][$experiment_field_name]);
			$query = "INSERT INTO {lab_migration_experiment} (proposal_id, number, title) VALUES (:proposal_id, :number, :experiment_field_name)";
			$args = array(
				":proposal_id" => $proposal_id,
				":number" => $number,
				":experiment_field_name" => trim($form_state['values'][$experiment_field_name])
			);
			$result = db_query($query, $args);
			if (!$result)
			{
				drupal_set_message(t('Could not insert Title of the Experiment : ') . trim($form_state['values'][$experiment_field_name]), 'error');
			} //!$result
			else
			{
				$number++;
			}
		} //strlen(trim($form_state['values'][$experiment_field_name])) >= 1
	} //$counter = 1; $counter <= 15; $counter++
	/* sending email */
	$email_to = $user->mail;
	$param['proposal_received']['proposal_id'] = $proposal_id;
	$param['proposal_received']['user_id'] = $user->uid;
	var_dump($param);
	if (!drupal_mail('lab_migration', 'proposal_received', $email_to, user_preferred_language($user), $param, variable_get('lab_migration_from_email', NULL), TRUE))
		drupal_set_message('Error sending email message.', 'error');
	/* sending email */
	$email_to = variable_get('lab_migration_emails', '');
	if (!drupal_mail('lab_migration', 'proposal_received', $email_to, user_preferred_language($user), $param, variable_get('lab_migration_from_email', NULL), TRUE))
		drupal_set_message('Error sending email message.', 'error');
	drupal_set_message(t('We have received you Lab migration proposal. We will get back to you soon.'), 'status');
	drupal_goto('');
}
