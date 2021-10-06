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

	$form['note'] = array(
		'#type' => 'item',
		'#markup' => "<span style='color:red;'> Note:</span> The experiments in the proposed lab should not be of very basic level
						and make sure that computational features of Scilab are used in each experiment. For example; Basic 						mathematics (BODMAS), Trigonometric functions (finding sine/cosine angle) etc. or the examples which are 							covered in Spoken Tutorial will not be accepted.<hr>",
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
	$form['semester_year'] = array(
		'#type' => 'select',
		'#title' => t('Year'),
		'#options' => array('First Year' => 'First Year',
							'Second Year' => 'Second Year',
							'Third Year' => 'Third Year',
							'Fourth Year' => 'Fourth Year'),
		'#required' => TRUE,
		'#description' => 'Please select year.'
	);
	$form['semester_details'] = array(
		'#type' => 'select',
		'#title' => t('Semester details'),
		'#options' => array('I' => 'I',
							'II' => 'II',
							'III' => 'III',
							'IV' => 'IV',
							'V' => 'V',
							'VI' => 'VI',
							'VII' => 'VII',
							'VIII' => 'VIII'),
		'#required' => TRUE,
		'#description' => 'Please select semester details'
	);
	$form['syllabus_copy'] = array(
		'#type' => 'fieldset',
		'#title' => t('<span class="form-required form-item" title="This field is required.">Syllabus copy (PDF) Files *</span>'),
		'#collapsible' => FALSE,
		'#collapsed' => FALSE
	);
	$form['syllabus_copy']['syllabus_copy_file'] = array(
		'#type' => 'file',
		'#title' => t('Upload pdf file'),
		'#size' => 48,
		'#description' => t('Separate filenames with underscore. No spaces or any special characters allowed in filename.') .
				'<br />' . t('<span style="color:red;">Allowed file extensions : ') .
							variable_get('lab_migration_syllabus_file_extensions', '') . '</span>'
	);
	$form['lab_title'] = array(
		'#title' => t('Title of the Lab'),
		'#type' => 'textfield',
		'#size' => 50,
		'#autocomplete_path' => 'lab_migration/lab_title/autocomplete',
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
				'#required' => TRUE,
				'#autocomplete_path' => 'lab_migration/lab_exp/autocomplete',
			);
		}else
		{
			$form['lab_experiment-' . $counter] = array(
				'#type' => 'textfield',
				'#title' => t('Title of the Experiment ') . $counter,
				'#size' => 50,
				'#required' => False,
				'#autocomplete_path' => 'lab_migration/lab_exp/autocomplete',
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
	$form['solution_display'] = array(
	'#type' => 'hidden',
	//  '#title' => t('Do you want to display the solution on the www.scilab.in website'),
	// '#options' => array('1' => 'Yes', '2' => 'No'),
	// '#required' => TRUE,
	'#default_value' => '2'
	//  '#description' => 'If yes, solutions will be made available to everyone for downloading.',
	//   '#disabled' => FALSE,
	);
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
  if (isset($_FILES['files']))
	{
		/* check if atleast one source or result file is uploaded */
		if (!($_FILES['files']['name']['syllabus_copy_file']))
			form_set_error('syllabus_copy_file', t('Please upload pdf file.'));
		/* check for valid filename extensions */
		foreach ($_FILES['files']['name'] as $file_form_name => $file_name)
		{
			if ($file_name)
			{
				/* checking file type */
				if (strstr($file_form_name, 'syllabus'))
					$file_type = 'S';
				else
					$file_type = 'U';
				$allowed_extensions_str = '';
				switch ($file_type)
				{
					case 'S':
						$allowed_extensions_str = variable_get('lab_migration_syllabus_file_extensions', '');
						break;
				} //$file_type
				$allowed_extensions = explode(',', $allowed_extensions_str);
				$allowd_file = strtolower($_FILES['files']['name'][$file_form_name]);
				$allowd_files = explode('.', $allowd_file);
				$temp_extension = end($allowd_files);
				if (!in_array($temp_extension, $allowed_extensions))
					form_set_error($file_form_name, t('Only file with ' . $allowed_extensions_str . ' extensions can be uploaded.'));
				if ($_FILES['files']['size'][$file_form_name] <= 0)
					form_set_error($file_form_name, t('File size cannot be zero.'));
				/* check if valid file name */
				if (!textbook_companion_check_valid_filename($_FILES['files']['name'][$file_form_name]))
					form_set_error($file_form_name, t('Invalid file name specified. Only alphabets and numbers are allowed as a valid filename.'));
			} //$file_name
		} //$_FILES['files']['name'] as $file_form_name => $file_name
	} //isset($_FILES['files'])
	return;
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
	$form_state['values']['lab_title'] = $form_state['values']['lab_title'];
	$form_state['values']['university'] = _lm_sentence_case($form_state['values']['university']);
	$solution_provider_uid = 0;
	$solution_status = 0;
	$solution_provider_name_title = '';
	$solution_provider_name = '';
	$solution_provider_contact_ph = '';
	$solution_provider_department = '';
	$solution_provider_university = '';
	$syllabus_copy_file_path = '';
	$message = '';
	$expected_completion_date = 0;
	$category = 0;
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
	$solution_display = 0;
	if ($form_state['values']['solution_display'] == "1")
		$solution_display = 1;
	else
		$solution_display = 0;
	/* inserting the user proposal */
	$v = $form_state["values"];
	$result = "INSERT INTO {lab_migration_proposal} (
	uid,
	approver_uid,
	solution_provider_uid,
	name_title,
	name,
	contact_ph,
	department,
	university,
	semester_year,
	semester_details,
	city,
	pincode,
	state,
	country,
	lab_title,
	category,
	approval_status,
	solution_status,
	solution_display,
	creation_date,
	approval_date,
	solution_date,
	expected_completion_date,
	message,
	solution_provider_name_title,
	solution_provider_name,
	solution_provider_contact_ph,
	solution_provider_department,
	solution_provider_university,
	syllabus_copy_file_path
	)
	VALUES (
	:uid,
	:approver_uid,
	:solution_provider_uid,
	:name_title,
	:name,
	:contact_ph,
	:department,
	:university,
	:semester_year,
	:semester_details,
	:city,
	:pincode,
	:state,
	:country,
	:lab_title,
	:category,
	:approval_status,
	:solution_status,

	:solution_display,
	:creation_date,
	:approval_date,
	:solution_date,
	:expected_completion_date,
	:message,
	:solution_provider_name_title,
	:solution_provider_name,
	:solution_provider_contact_ph,
	:solution_provider_department,
	:solution_provider_university,
	:syllabus_copy_file_path
	)";
	$args = array(
		":uid" => $user->uid,
		":approver_uid" => 0,
		":name_title" => $v['name_title'],
		":name" => $v['name'],
		":contact_ph" => $v['contact_ph'],
		":department" => $v['department'],
		":university" => $v['university'],
		":semester_year" => $v['semester_year'],
		":semester_details" => $v['semester_details'],
		":city" => $v['city'],
		":pincode" => $v['pincode'],
		":state" => $v['all_state'],
		":country" => "India",
		":lab_title" => $v['lab_title'],
		":category" => $category,
		":approval_status" => 0,
		":solution_status" => $solution_status,
		":solution_provider_uid" => $solution_provider_uid,
		":solution_display" => $solution_display,
		":creation_date" => time(),
		":approval_date" => 0,
		":solution_date" => 0,
		":expected_completion_date" => $expected_completion_date,
		":message" => $message,
		":solution_provider_name_title" => $solution_provider_name_title,
		":solution_provider_name" => $solution_provider_name,
		":solution_provider_contact_ph" => $solution_provider_contact_ph,
		":solution_provider_department" => $solution_provider_department,
		":solution_provider_university" => $solution_provider_university,
		":syllabus_copy_file_path" => "",
	);
	$proposal_id = db_query($result, $args, array(
		'return' => Database::RETURN_INSERT_ID
	));
	$root_path = lab_migration_path();
	$dest_path = $proposal_id . '/';
	if (!is_dir($root_path . $dest_path))
		mkdir($root_path . $dest_path);
	/* uploading files */
	foreach ($_FILES['files']['name'] as $file_form_name => $file_name)
	{
		if ($file_name)
		{
			/* checking file type */
			$file_type = 'S';
			if (file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]))
			{
				drupal_set_message(t("Error uploading file. File !filename already exists.", array(
					'!filename' => $_FILES['files']['name'][$file_form_name]
				)), 'error');
				return;
			} //file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
			/* uploading file */
			if (move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . $_FILES['files']['name'][$file_form_name]))
			{
				$query = "UPDATE {lab_migration_proposal} SET syllabus_copy_file_path = :syllabus_copy_file_path WHERE id = :id";
				$args = array(
					":syllabus_copy_file_path" => $dest_path . $_FILES['files']['name'][$file_form_name],
					":id" => $proposal_id
				);
				$updateresult = db_query($query, $args);
				drupal_set_message($file_name . ' uploaded successfully.', 'status');
			} //move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
			else
			{
				drupal_set_message('Error uploading file : ' . $dest_path . '/' . $file_name, 'error');
			}
		} //$file_name
	} //$_FILES['files']['name'] as $file_form_name => $file_name
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
			$form_state['values'][$experiment_field_name] = $form_state['values'][$experiment_field_name];
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
	$params['proposal_received']['proposal_id'] = $proposal_id;
	$params['proposal_received']['user_id'] = $user->uid;

		$email_to = $user->mail;
		$from = variable_get('lab_migration_from_email', '');
		$bcc = variable_get('lab_migration_bcc_email', '');
		$cc = variable_get('lab_migration_emails', '');
		$params['proposal_received']['headers'] = array(
			'From' => $from,
			'MIME-Version' => '1.0',
			'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
			'Content-Transfer-Encoding' => '8Bit',
			'X-Mailer' => 'Drupal',
			'Cc' => $cc,
			'Bcc' => $bcc,
		);
		if (!drupal_mail('lab_migration', 'proposal_received', $email_to, language_default(), $params, $from, TRUE))
			drupal_set_message('Error sending email message.', 'error');

	drupal_set_message(t('We have received you Lab migration proposal. We will get back to you soon.'), 'status');
	drupal_goto('');
}


function lab_migration_add_experiment_form($form, &$form_state)
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
	
	$form['#attributes'] = array(
		'enctype' => "multipart/form-data"
	);

	
	$first_experiemnt = TRUE;
	for ($counter = 1; $counter <= 15; $counter++)
	{
		if($counter<= variable_get('lab_migration_minimum_exp_limit', '')){
			$form['lab_experiment-' . $counter] = array(
				'#type' => 'textfield',
				'#title' => t('Title of the Experiment ') . $counter,
				'#size' => 50,
				'#required' => TRUE,
				'#autocomplete_path' => 'lab_migration/lab_exp/autocomplete',
			);
		}else
		{
			$form['lab_experiment-' . $counter] = array(
				'#type' => 'textfield',
				'#title' => t('Title of the Experiment ') . $counter,
				'#size' => 50,
				'#required' => False,
				'#autocomplete_path' => 'lab_migration/lab_exp/autocomplete',
			);
		}
		$first_experiemnt = FALSE;
	} //$counter = 1; $counter <= 15; $counter++

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	return $form;
}


function lab_migration_add_experiment_form_submit($form, &$form_state)
{
	global $user;
	$proposal_id = $_GET['pid'];
	$number = 1;
	for ($counter = 1; $counter <= 15; $counter++)
	{
		$experiment_field_name = 'lab_experiment-' . $counter;
		if (strlen(trim($form_state['values'][$experiment_field_name])) >= 1)
		{
			$form_state['values'][$experiment_field_name] = $form_state['values'][$experiment_field_name];
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
	
	drupal_set_message(t('Experiments added successfully.'), 'status');
	drupal_goto('/lab_migration/admin/code-list/'.$proposal_id);
}

function lab_migration_add_experiment_one_form($form, &$form_state)
{
	global $user;
	/************************ start approve book details ************************/
	$form['lab_experiment'] = array(
		'#type' => 'textfield',
		'#title' => t('Title of the Experiment ') . $counter,
		'#size' => 50,
		'#required' => TRUE,
		'#autocomplete_path' => 'lab_migration/lab_exp/autocomplete',
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	return $form;
}


function lab_migration_add_experiment_one_form_submit($form, &$form_state)
{
	global $user;
	$proposal_id = $_GET['pid'];
	$number = $_GET['number'];

	$query = db_select('lab_migration_experiment','l');
	$query->fields('l',['number', 'id']);
	$query->condition('l.proposal_id', $proposal_id);
	$proposal_q = $query->execute();
	$proposal_data = $proposal_q->fetchAll();

	foreach($proposal_data as $value) {

		if($value->number > $number) {
			$num_updated = db_update('lab_migration_experiment') // Table name no longer needs {}
			  ->fields(array(
			    'number' => $value->number + 1,
			  ))
			  ->condition('id', $value->id, '=')
			  ->execute();
		}
		
	}

	$number = $number + 1;
	
	$query = "INSERT INTO {lab_migration_experiment} (proposal_id, number, title) VALUES (:proposal_id, :number, :experiment_field_name)";
	$args = array(
		":proposal_id" => $proposal_id,
		":number" => $number,
		":experiment_field_name" => trim($form_state['values']['lab_experiment'])
	);
	$result = db_query($query, $args);
			
	drupal_set_message(t('Experiment added successfully.'), 'status');
	drupal_goto('/lab_migration/admin/code-list/'.$proposal_id);
}