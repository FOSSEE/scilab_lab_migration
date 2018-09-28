<?php
// $Id$
function lab_migration_code_approval()
{
	/* get a list of unapproved solutions */
	//$pending_solution_q = db_query("SELECT * FROM {lab_migration_solution} WHERE approval_status = 0");
	$query = db_select('lab_migration_solution');
	$query->fields('lab_migration_solution');
	$query->condition('approval_status', 0);
	$pending_solution_q = $query->execute();
	if (!$pending_solution_q)
	{
		drupal_set_message(t('There are no pending code approvals.'), 'status');
		return '';
	} //!$pending_solution_q
	$pending_solution_rows = array();
	while ($pending_solution_data = $pending_solution_q->fetchObject())
	{
		/* get experiment data */
		//$experiment_q = db_query("SELECT * FROM {lab_migration_experiment} WHERE id = %d", $pending_solution_data->experiment_id);
		$query = db_select('lab_migration_experiment');
		$query->fields('lab_migration_experiment');
		$query->condition('id', $pending_solution_data->experiment_id);
		$experiment_q = $query->execute();
		$experiment_data = $experiment_q->fetchObject();
		/* get proposal data */
		// $proposal_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE id = %d", $experiment_data->proposal_id);
		$query = db_select('lab_migration_proposal');
		$query->fields('lab_migration_proposal');
		$query->condition('id', $experiment_data->proposal_id);
		$proposal_q = $query->execute();
		$proposal_data = $proposal_q->fetchObject();
		/* get solution provider details */
		$solution_provider_user_name = '';
		$user_data = user_load($proposal_data->solution_provider_uid);
		if ($user_data)
		{
			$solution_provider_user_name = $user_data->name;
		} //$user_data
		else
		{
			$solution_provider_user_name = '';
		}
		/* setting table row information */
		$pending_solution_rows[] = array(
			$proposal_data->lab_title,
			$experiment_data->title,
			$proposal_data->name,
			$solution_provider_user_name,
			l('Edit', 'lab_migration/code_approval/approve/' . $pending_solution_data->id)
		);
	} //$pending_solution_data = $pending_solution_q->fetchObject()
	/* check if there are any pending solutions */
	if (!$pending_solution_rows)
	{
		drupal_set_message(t('There are no pending solutions'), 'status');
		return '';
	} //!$pending_solution_rows
	$header = array(
		'Title of the Lab',
		'Experiment',
		'Proposer',
		'Solution Provider',
		'Actions'
	);
	//$output = theme_table($header, $pending_solution_rows);
	$output = theme('table', array(
		'header' => $header,
		'rows' => $pending_solution_rows
	));
	return $output;
}
function lab_migration_code_approval_form($form_state)
{
	$solution_id = (int) arg(3);
	/* get solution details */
	//$solution_q = db_query("SELECT * FROM {lab_migration_solution} WHERE id = %d", $solution_id);
	$query = db_select('lab_migration_solution');
	$query->fields('lab_migration_solution');
	$query->condition('id', $solution_id);
	$solution_q = $query->execute();
	$solution_data = $solution_q->fetchObject();
	if (!$solution_data)
	{
		drupal_set_message(t('Invalid solution selected.'), 'status');
		drupal_goto('lab_migration/code_approval');
	} //!$solution_data
	if ($solution_data->approval_status == 1)
	{
		drupal_set_message(t('This solution has already been approved. Are you sure you want to change the approval status?'), 'error');
	} //$solution_data->approval_status == 1
	if ($solution_data->approval_status == 2)
	{
		drupal_set_message(t('This solution has already been dis-approved. Are you sure you want to change the approval status?'), 'error');
	} //$solution_data->approval_status == 2
	/* get experiment data */
	//xperiment_q = db_query("SELECT * FROM {lab_migration_experiment} WHERE id = %d", $solution_data->experiment_id);
	$query = db_select('lab_migration_experiment');
	$query->fields('lab_migration_experiment');
	$query->condition('id', $solution_data->experiment_id);
	$experiment_q = $query->execute();
	$experiment_data = $experiment_q->fetchObject();
	/* get proposal data */
	//$proposal_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE id = %d", $experiment_data->proposal_id);
	$query = db_select('lab_migration_proposal');
	$query->fields('lab_migration_proposal');
	$query->condition('id', $experiment_data->proposal_id);
	$proposal_q = $query->execute();
	$proposal_data = $proposal_q->fetchObject();
	/* get solution provider details */
	$solution_provider_user_name = '';
	$user_data = user_load($proposal_data->solution_provider_uid);
	if ($user_data)
	{
		$solution_provider_user_name = $user_data->name;
	} //$user_data
	else
	{
		$solution_provider_user_name = '';
	}
	$form['#tree'] = TRUE;
	$form['lab_title'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->lab_title,
		'#title' => t('Title of the Lab')
	);
	$form['name'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->name,
		'#title' => t('Contributor Name')
	);
	$form['experiment']['number'] = array(
		'#type' => 'item',
		'#markup' => $experiment_data->number,
		'#title' => t('Experiment Number')
	);
	$form['experiment']['title'] = array(
		'#type' => 'item',
		'#markup' => $experiment_data->title,
		'#title' => t('Title of the Experiment')
	);
	$form['back_to_list'] = array(
		'#type' => 'item',
		'#markup' => l('Back to Code Approval List', 'lab_migration/code_approval')
	);
	$form['code_number'] = array(
		'#type' => 'item',
		'#markup' => $solution_data->code_number,
		'#title' => t('Code No')
	);
	$form['code_caption'] = array(
		'#type' => 'item',
		'#markup' => $solution_data->caption,
		'#title' => t('Caption')
	);
	/* get solution files */
	$solution_files_html = '';
	//$solution_files_q = db_query("SELECT * FROM {lab_migration_solution_files} WHERE solution_id = %d ORDER BY id ASC", $solution_id);
	$query = db_select('lab_migration_solution_files');
	$query->fields('lab_migration_solution_files');
	$query->condition('solution_id', $solution_id);
	$query->orderBy('id', 'ASC');
	$solution_files_q = $query->execute();
	if ($solution_files_q)
	{
		while ($solution_files_data = $solution_files_q->fetchObject())
		{
			$code_file_type = '';
			switch ($solution_files_data->filetype)
			{
				case 'S':
					$code_file_type = 'Source';
					break;
				case 'R':
					$code_file_type = 'Result';
					break;
				case 'X':
					$code_file_type = 'Xcox';
					break;
				case 'U':
					$code_file_type = 'Unknown';
					break;
				default:
					$code_file_type = 'Unknown';
					break;
			} //$solution_files_data->filetype
			$solution_files_html .= l($solution_files_data->filename, 'lab_migration/download/file/' . $solution_files_data->id) . ' (' . $code_file_type . ')' . '<br/>';
		} //$solution_files_data = $solution_files_q->fetchObject()
	} //$solution_files_q
	/* get dependencies files */
	//$dependency_q = db_query("SELECT * FROM {lab_migration_solution_dependency} WHERE solution_id = %d ORDER BY id ASC", $solution_id);
	$query = db_select('lab_migration_solution_dependency');
	$query->fields('lab_migration_solution_dependency');
	$query->condition('solution_id', $solution_id);
	$query->orderBy('id', 'ASC');
	$dependency_q = $query->execute();
	while ($dependency_data = $dependency_q->fetchObject())
	{
		//$dependency_files_q = db_query("SELECT * FROM {lab_migration_dependency_files} WHERE id = %d", $dependency_data->dependency_id);
		$query = db_select('lab_migration_dependency_files');
		$query->fields('lab_migration_dependency_files');
		$query->condition('id', $dependency_data->dependency_id);
		$dependency_files_q = $query->execute();
		$dependency_files_data = $dependency_files_q->fetchObject();
		$solution_file_type = 'Dependency file';
		$solution_files_html .= l($dependency_files_data->filename, 'lab_migration/download/dependency/' . $dependency_files_data->id) . ' (' . 'Dependency' . ')' . '<br/>';
	} //$dependency_data = $dependency_q->fetchObject()
	$form['solution_files'] = array(
		'#type' => 'item',
		'#markup' => $solution_files_html,
		'#title' => t('Solution')
	);
	$form['approved'] = array(
		'#type' => 'radios',
		'#options' => array(
			'0' => 'Pending',
			'1' => 'Approved',
			'2' => 'Dis-approved (Solution will be deleted)'
		),
		'#title' => t('Approval'),
		'#default_value' => $solution_data->approval_status
	);
	$form['message'] = array(
		'#type' => 'textarea',
		'#title' => t('Reason for dis-approval')
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	$form['cancel'] = array(
		'#type' => 'markup',
		'#markup' => l(t('Cancel'), 'lab_migration/code_approval')
	);
	return $form;
}
function lab_migration_code_approval_form_submit($form, &$form_state)
{
	global $user;
	$solution_id = (int) arg(3);
	/* get solution details */
	//$solution_q = db_query("SELECT * FROM {lab_migration_solution} WHERE id = %d", $solution_id);
	$query = db_select('lab_migration_solution');
	$query->fields('lab_migration_solution');
	$query->condition('id', $solution_id);
	$solution_q = $query->execute();
	$solution_data = $solution_q->fetchObject();
	if (!$solution_data)
	{
		drupal_set_message(t('Invalid solution selected.'), 'status');
		drupal_goto('lab_migration/code_approval');
	} //!$solution_data
	/* get experiment data */
	//$experiment_q = db_query("SELECT * FROM {lab_migration_experiment} WHERE id = %d", $solution_data->experiment_id);
	$query = db_select('lab_migration_experiment');
	$query->fields('lab_migration_experiment');
	$query->condition('id', $solution_data->experiment_id);
	$experiment_q = $query->execute();
	$experiment_data = $experiment_q->fetchObject();
	/* get proposal data */
	//$proposal_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE id = %d", $experiment_data->proposal_id);
	$query = db_select('lab_migration_proposal');
	$query->fields('lab_migration_proposal');
	$query->condition('id', $experiment_data->proposal_id);
	$proposal_q = $query->execute();
	$proposal_data = $proposal_q->fetchObject();
	$user_data = user_load($proposal_data->uid);
	$solution_prove_user_data = user_load($proposal_data->solution_provider_uid);
	// **** TODO **** : del_lab_pdf($proposal_data->id);
	if ($form_state['values']['approved'] == "0")
	{
		$query = "UPDATE {lab_migration_solution} SET approval_status = 0, approver_uid = :approver_uid, approval_date = :approval_date WHERE id = :solution_id";
		$args = array(
			":approver_uid" => $user->uid,
			":approval_date" => time(),
			":solution_id" => $solution_id
		);
		db_query($query, $args);
		/* sending email */
		$email_to = $user_data->mail . ', ' . variable_get('lab_migration_emails', '');
		$params['solution_pending']['solution_id'] = $solution_id;
		$params['solution_pending']['user_id'] = $user_data->uid;
		if (!drupal_mail('lab_migration', 'solution_pending', $email_to, language_default(), $params, variable_get('lab_migration_from_email', NULL), TRUE))
			drupal_set_message('Error sending email message.', 'error');
	} //$form_state['values']['approved'] == "0"
	else if ($form_state['values']['approved'] == "1")
	{
		$query = "UPDATE {lab_migration_solution} SET approval_status = 1, approver_uid = :approver_uid, approval_date = :approval_date WHERE id = :solution_id";
		$args = array(
			":approver_uid" => $user->uid,
			":approval_date" => time(),
			":solution_id" => $solution_id
		);
		db_query($query, $args);
		/* sending email */
		$email_to = $user_data->mail;
		$from = variable_get('lab_migration_from_email', '');
		$bcc = variable_get('lab_migration_bcc_email', '');
		$cc = variable_get('lab_migration_from_email', '');
		$params['solution_approved']['headers'] = array(
			'From' => $from,
			'MIME-Version' => '1.0',
			'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
			'Content-Transfer-Encoding' => '8Bit',
			'X-Mailer' => 'Drupal',
			'Cc' => $cc,
			'Bcc' => $bcc
		);
		if (!drupal_mail('lab_migration', 'solution_approved', $email_to, language_default(), $params, $from, TRUE))
			drupal_set_message('Error sending email message.', 'error');


	} //$form_state['values']['approved'] == "1"
	else if ($form_state['values']['approved'] == "2")
	{
		if (lab_migration_delete_solution($solution_id))
		{
			$params['solution_disapproved']['experiment_number'] = $experiment_data->number;
			$params['solution_disapproved']['experiment_title'] = $experiment_data->title;
			$params['solution_disapproved']['solution_number'] = $solution_data->code_number;
			$params['solution_disapproved']['solution_caption'] = $solution_data->caption;
			$params['solution_disapproved']['user_id'] = $user_data->uid;
			$params['solution_disapproved']['message'] = $form_state['values']['message'];
			/* sending email */
			$email_to = $user_data->mail;
			$from = variable_get('lab_migration_from_email', '');
			$bcc = variable_get('lab_migration_bcc_email', '');
			$cc = variable_get('lab_migration_from_email', '');
			$params['solution_approved']['headers'] = array(
				'From' => $from,
				'MIME-Version' => '1.0',
				'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
				'Content-Transfer-Encoding' => '8Bit',
				'X-Mailer' => 'Drupal',
				'Cc' => $cc,
				'Bcc' => $bcc
			);
			if (!drupal_mail('lab_migration', 'solution_disapproved', $email_to, language_default(), $params, $from, TRUE))
				drupal_set_message('Error sending email message.', 'error');

		} //lab_migration_delete_solution($solution_id)
		else
		{
			drupal_set_message('Error disapproving and deleting solution. Please contact administrator.', 'error');
		}
	} //$form_state['values']['approved'] == "2"
	drupal_set_message('Updated successfully.', 'status');
	drupal_goto('lab_migration/code_approval');
}
