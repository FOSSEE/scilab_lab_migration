<?php
/******************************************************************************/
/***************************** DELETE CODE ************************************/
/******************************************************************************/
function lab_migration_upload_code_delete()
{
	global $user;
	$root_path = lab_migration_path();
	$solution_id = (int) arg(3);
	/* check solution */
	// $solution_q = db_query("SELECT * FROM {lab_migration_solution} WHERE id = %d LIMIT 1", $solution_id);
	$query = db_select('lab_migration_solution');
	$query->fields('lab_migration_solution');
	$query->condition('id', $solution_id);
	$query->range(0, 1);
	$solution_q = $query->execute();
	$solution_data = $solution_q->fetchObject();
	if (!$solution_data)
	{
		drupal_set_message('Invalid solution.', 'error');
		drupal_goto('lab_migration/code');
		return;
	} //!$solution_data
	if ($solution_data->approval_status != 0)
	{
		drupal_set_message('You cannnot delete a solution after it has been approved. Please contact site administrator if you want to delete this solution.', 'error');
		drupal_goto('lab_migration/code');
		return;
	} //$solution_data->approval_status != 0
	//$experiment_q = db_query("SELECT * FROM {lab_migration_experiment} WHERE id = %d LIMIT 1", $solution_data->experiment_id);
	$query = db_select('lab_migration_experiment');
	$query->fields('lab_migration_experiment');
	$query->condition('id', $solution_data->experiment_id);
	$query->range(0, 1);
	$experiment_q = $query->execute();
	$experiment_data = $experiment_q->fetchObject();
	if (!$experiment_data && !in_array('administrator', $user->roles))
	{
		drupal_set_message('You do not have permission to delete this solution.', 'error');
		drupal_goto('lab_migration/code');
		return;
	} //!$experiment_data
	//$proposal_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE id = %d AND solution_provider_uid = %d LIMIT 1", $experiment_data->proposal_id, $user->uid);
	$query = db_select('lab_migration_proposal');
	$query->fields('lab_migration_proposal');
	$query->condition('id', $experiment_data->proposal_id);
	$query->condition('solution_provider_uid', $user->uid);
	$query->range(0, 1);
	$proposal_q = $query->execute();
	$proposal_data = $proposal_q->fetchObject();
	if (!$proposal_data && !in_array('administrator', $user->roles))
	{
		drupal_set_message('You do not have permission to delete this solution.', 'error');
		drupal_goto('lab_migration/code');
		return;
	} //!$proposal_data
	/* deleting solution files */
		$params['solution_deleted_user']['lab_title'] = $proposal_data->lab_title;
		$params['solution_deleted_user']['experiment_title'] = $experiment_data->title;
		$params['solution_deleted_user']['solution_number'] = $solution_data->code_number;
		$params['solution_deleted_user']['solution_caption'] = $solution_data->caption;
		$params['solution_deleted_user']['user_id'] = $user->uid;
		//var_dump($param['solution_deleted_user']);die;
	if (lab_migration_delete_solution($solution_data->id))
	{
		drupal_set_message('Solution deleted.', 'status');
		/* sending email */
		$email_to = $user->mail;
		$from = variable_get('lab_migration_from_email', '');
		$bcc = variable_get('lab_migration_bcc_email', '');
		$cc = variable_get('lab_migration_from_email', '');
		$params['solution_deleted_user']['headers'] = array(
			'From' => $from,
			'MIME-Version' => '1.0',
			'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
			'Content-Transfer-Encoding' => '8Bit',
			'X-Mailer' => 'Drupal',
			'Cc' => $cc,
			'Bcc' => $bcc
		);
		if (!drupal_mail('lab_migration', 'solution_deleted_user', $email_to, language_default(), $params, $from, TRUE))
			drupal_set_message('Error sending email message.', 'error');

	} //lab_migration_delete_solution($solution_data->id)
	else
	{
		drupal_set_message('Error deleting example.', 'status');
	}
	$id = $_GET['id'];
	drupal_goto('lab_migration/admin/code-list/'.$id);
	return;
}

function lab_migration_experiment_delete() {
	global $base_url;
	$id = $_GET['id'];
	$exp_id = arg(3);
	$query = db_select('lab_migration_solution', 'l');
	$query->fields('l', ['id']);
	$query->condition('experiment_id', $exp_id);
	$solution_q = $query->execute()->fetchAll();
	
	$num_deleted = db_delete('lab_migration_experiment')
	  ->condition('id', $exp_id)
	  ->execute();

	$num_deleted = db_delete('lab_migration_solution')
	  ->condition('experiment_id', $exp_id)
	  ->execute();

	$num_deleted = db_delete('lab_migration_solution_files')
	  ->condition('solution_id', $solution_q[0]->id)
	  ->execute();
	drupal_set_message('Experiment successfully deleted.');
	drupal_goto($base_url . '/lab_migration/admin/code-list/'.$id);
}
?>
