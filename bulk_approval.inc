<?php
function lab_migration_bulk_approval_form($form, &$form_state)
{
	$options_first = _bulk_list_of_labs();
	$options_two = _ajax_bulk_get_experiment_list();
	$selected = isset($form_state['values']['lab']) ? $form_state['values']['lab'] : key($options_first);
	$select_two = isset($form_state['values']['lab_experiment_list']) ? $form_state['values']['lab_experiment_list'] : key($options_two);
	$form = array();
	$form['lab'] = array(
		'#type' => 'select',
		'#title' => t('Title of the lab'),
		'#options' => _bulk_list_of_labs(),
		'#default_value' => $selected,
		'#ajax' => array(
			'callback' => 'ajax_bulk_experiment_list_callback'
		),
		'#suffix' => '<div id="ajax_selected_lab"></div><div id="ajax_selected_lab_pdf"></div>'
	);
	$form['lab_actions'] = array(
		'#type' => 'select',
		'#title' => t('Please select action'),
		//'#options' => _bulk_list_lab_actions(),
		'#default_value' => 0,
		'#prefix' => '<div id="ajax_selected_lab_action" style="color:red;">',
		'#suffix' => '</div>',
		'#states' => array(
			'invisible' => array(
				':input[name="lab"]' => array(
					'value' => 0
				)
			)
		)
	);
	$form['lab_experiment_list'] = array(
		'#type' => 'select',
		'#title' => t('Titile of the experiment'),
		'#options' => _ajax_bulk_get_experiment_list($selected),
		'#default_value' => isset($form_state['values']['lab_experiment_list']) ? $form_state['values']['lab_experiment_list'] : '',
		'#ajax' => array(
			'callback' => 'ajax_bulk_solution_list_callback'
		),
		'#prefix' => '<div id="ajax_selected_experiment">',
		'#suffix' => '</div>',
		'#states' => array(
			'invisible' => array(
				':input[name="lab"]' => array(
					'value' => 0
				)
			)
		)
	);
	$form['download_experiment'] = array(
		'#type' => 'item',
		'#markup' => '<div id="ajax_download_experiment"></div>'
	);
	$form['lab_experiment_actions'] = array(
		'#type' => 'select',
		'#title' => t('Please select action'),
		'#options' => _bulk_list_experiment_actions(),
		'#default_value' => 0,
		'#prefix' => '<div id="ajax_selected_lab_experiment_action" style="color:red;">',
		'#suffix' => '</div>',
		'#states' => array(
			'invisible' => array(
				':input[name="lab"]' => array(
					'value' => 0
				)
			)
		)
	);
	$form['lab_solution_list'] = array(
		'#type' => 'select',
		'#title' => t('Solution'),
		'#options' => _ajax_bulk_get_solution_list($select_two),
		'#default_value' => isset($form_state['values']['lab_solution_list']) ? $form_state['values']['lab_solution_list'] : '',
		'#ajax' => array(
			'callback' => 'ajax_bulk_solution_files_callback'
		),
		'#prefix' => '<div id="ajax_selected_solution">',
		'#suffix' => '</div>',
		'#states' => array(
			'invisible' => array(
				':input[name="lab"]' => array(
					'value' => 0
				)
			)
		)
	);
	$form['lab_experiment_solution_actions'] = array(
		'#type' => 'select',
		'#title' => t('Please select action'),
		'#options' => _bulk_list_solution_actions(),
		'#default_value' => 0,
		'#prefix' => '<div id="ajax_selected_lab_experiment_solution_action" style="color:red;">',
		'#suffix' => '</div>',
		'#states' => array(
			'invisible' => array(
				':input[name="lab"]' => array(
					'value' => 0
				)
			)
		)
	);
	$form['download_solution'] = array(
		'#type' => 'item',
		'#markup' => '<div id="ajax_download_experiment_solution"></div>'
	);
	$form['edit_solution'] = array(
		'#type' => 'item',
		'#markup' => '<div id="ajax_edit_experiment_solution"></div>'
	);
	$form['solution_files'] = array(
		'#type' => 'item',
		//  '#title' => t('List of solution_files'),
		'#markup' => '<div id="ajax_solution_files"></div>',
		'#states' => array(
			'invisible' => array(
				':input[name="lab"]' => array(
					'value' => 0
				)
			)
		)
	);
	$form['message'] = array(
		'#type' => 'textarea',
		'#title' => t('If Dis-Approved please specify reason for Dis-Approval'),
		'#prefix' => '<div id= "message_submit">',
		'#states' => array(
			'invisible' => array(
				':input[name="lab"]' => array(
					'value' => 0
				)
			)
		)
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
		'#suffix' => '</div>',
		'#states' => array(
			'invisible' => array(
				':input[name="lab"]' => array(
					'value' => 0
				)
			)
		)
	);
	return $form;
}
/**
 * Ajax callback triggered by select.
 */
function ajax_bulk_experiment_list_callback($form, $form_state)
{
	$commands = array();
	$lab_default_value = $form_state['values']['lab'];
	if ($lab_default_value != 0)
	{
		$commands[] = ajax_command_html('#ajax_selected_lab', l('Download', 'lab_migration/full_download/lab/' . $lab_default_value) . ' ' . t('(Download all the approved and unapproved solutions of the entire lab)'));
		$commands[] = ajax_command_html('#ajax_selected_lab_pdf', l('Download PDF', 'lab_migration/generate_lab/' . $lab_default_value . '/1') . ' ' . t('(Download PDF of all the approved and unapproved solution of the entire lab)'));
		$form['lab_actions']['#options'] = _bulk_list_lab_actions();
		$form['lab_experiment_list']['#options'] = _ajax_bulk_get_experiment_list($lab_default_value);
		$commands[] = ajax_command_data('#ajax_selected_lab', 'form_state_value_select', $form_state['values']['lab_experiment_list']);
		$commands[] = ajax_command_replace('#ajax_selected_experiment', drupal_render($form['lab_experiment_list']));
		$commands[] = ajax_command_replace('#ajax_selected_lab_action', drupal_render($form['lab_actions']));
		$commands[] = ajax_command_html('#ajax_selected_solution', '');
		$commands[] = ajax_command_html('#ajax_selected_lab_experiment_action', '');
		$commands[] = ajax_command_html('#ajax_selected_lab_experiment_solution_action', '');
		$commands[] = ajax_command_html('#ajax_solution_files', '');
		$commands[] = ajax_command_html('#ajax_download_experiment_solution', '');
		$commands[] = ajax_command_html('#ajax_edit_experiment_solution', '');
	} //$lab_default_value != 0
	else
	{
		$commands[] = ajax_command_html('#ajax_selected_lab', '');
		$commands[] = ajax_command_html('#ajax_selected_lab_pdf', '');
		$commands[] = ajax_command_data('#ajax_selected_lab', 'form_state_value_select', $form_state['values']['lab']);
		$commands[] = ajax_command_html('#ajax_selected_experiment', '');
		$commands[] = ajax_command_html('#ajax_selected_lab_action', '');
		$commands[] = ajax_command_html('#ajax_selected_lab_experiment_action', '');
		$commands[] = ajax_command_html('#ajax_download_experiment', '');
		$commands[] = ajax_command_html('#ajax_selected_lab_experiment_solution_action', '');
		$commands[] = ajax_command_html('#ajax_solution_files', '');
		$commands[] = ajax_command_html('#ajax_download_experiment_solution', '');
		$commands[] = ajax_command_html('#ajax_edit_experiment_solution', '');
	}
	return array(
		'#type' => 'ajax',
		'#commands' => $commands
	);
}
function ajax_bulk_solution_list_callback($form, $form_state)
{
	$commands = array();
	$experiment_list_default_value = $form_state['values']['lab_experiment_list'];
	if ($experiment_list_default_value != 0)
	{
		$form['lab_experiment_actions']['#options'] = _bulk_list_experiment_actions();
		$form['lab_solution_list']['#options'] = _ajax_bulk_get_solution_list($experiment_list_default_value);
		$commands[] = ajax_command_html('#ajax_download_experiment', l('Download Experiment', 'lab_migration/download/experiment/' . $experiment_list_default_value));
		$commands[] = ajax_command_data('#ajax_selected_experiment', 'form_state_value_select', $form_state['values']['lab_experiment_list']);
		$commands[] = ajax_command_html('#ajax_selected_experiment', drupal_render($form['lab_experiment_list']));
		$commands[] = ajax_command_html('#ajax_selected_lab_experiment_action', drupal_render($form['lab_experiment_actions']));
		$commands[] = ajax_command_html('#ajax_selected_solution', drupal_render($form['lab_solution_list']));
		$commands[] = ajax_command_html('#ajax_selected_lab_experiment_solution_action', '');
		$commands[] = ajax_command_html('#ajax_solution_files', '');
		$commands[] = ajax_command_html('#ajax_download_experiment_solution', '');
		$commands[] = ajax_command_html('#ajax_edit_experiment_solution', '');
	} //$experiment_list_default_value != 0
	else
	{
		$commands[] = ajax_command_html('#ajax_download_experiment', '');
		$commands[] = ajax_command_html('#ajax_selected_lab_experiment_action', '');
		$commands[] = ajax_command_html('#ajax_selected_solution', '');
		$commands[] = ajax_command_html('#ajax_selected_lab_experiment_solution_action', '');
		$commands[] = ajax_command_html('#ajax_solution_files', '');
		$commands[] = ajax_command_html('#ajax_download_experiment_solution', '');
		$commands[] = ajax_command_html('#ajax_edit_experiment_solution', '');
		// $commands[] = ajax_command_replace('#ajax_selected_experiment',drupal_render($form['lab_experiment_list']));
	}
	return array(
		'#type' => 'ajax',
		'#commands' => $commands
	);
}
function ajax_bulk_solution_files_callback($form, $form_state)
{
	$commands = array();
	$solution_list_default_value = $form_state['values']['lab_solution_list'];
	if ($solution_list_default_value != 0)
	{
		$form['lab_experiment_solution_actions']['#options'] = _bulk_list_solution_actions();
		$commands[] = ajax_command_html('#ajax_selected_lab_experiment_solution_action', drupal_render($form['lab_experiment_solution_actions']));
		/*************************************************************************************/
		//$solution_list_q = db_query("SELECT * FROM {lab_migration_solution_files} WHERE solution_id = %d", $form_state['values']['solution']);
		$query = db_select('lab_migration_solution_files');
		$query->fields('lab_migration_solution_files');
		$query->condition('solution_id', $solution_list_default_value);
		$solution_list_q = $query->execute();
		if ($solution_list_q)
		{
			$solution_files_rows = array();
			while ($solution_list_data = $solution_list_q->fetchObject())
			{
				$solution_file_type = '';
				switch ($solution_list_data->filetype)
				{
					case 'S':
						$solution_file_type = 'Source or Main file';
						break;
					case 'R':
						$solution_file_type = 'Result file';
						break;
					case 'X':
						$solution_file_type = 'xcos file';
						break;
					default:
						$solution_file_type = 'Unknown';
						break;
				} //$solution_list_data->filetype
				$solution_files_rows[] = array(
					l($solution_list_data->filename, 'lab_migration/download/file/' . $solution_list_data->id),
					$solution_file_type
				);
			} //$solution_list_data = $solution_list_q->fetchObject()
			/* dependency files */
			//$dependency_q = db_query("SELECT * FROM {lab_migration_solution_dependency} WHERE solution_id = %d", $form_state['values']['solution']);
			$query = db_select('lab_migration_solution_dependency');
			$query->fields('lab_migration_solution_dependency');
			$query->condition('solution_id', $solution_list_default_value);
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
				$solution_files_rows[] = array(
					l($dependency_files_data->filename, 'lab_migration/download/dependency/' . $dependency_files_data->dependency_id),
					$solution_file_type
				);
			} //$dependency_data = $dependency_q->fetchObject()
			/* creating list of files table */
			$solution_files_header = array(
				'Filename',
				'Type'
			);
			$solution_files = theme('table', array(
				'header' => $solution_files_header,
				'rows' => $solution_files_rows
			));
		} //$solution_list_q
		$form['solution_files']['#title'] = 'List of solution files';
		$form['solution_files']['#markup'] = $solution_files;
		$commands[] = ajax_command_html('#ajax_download_experiment_solution', l('Download Solution', 'lab_migration/download/solution/' . $solution_list_default_value));
		$commands[] = ajax_command_html('#ajax_edit_experiment_solution', l('Edit Solution', 'code_approval/editcode/' . $solution_list_default_value));
		// $commands[] = ajax_command_html('#ajax_solution_files', $solution_files);
		$commands[] = ajax_command_html('#ajax_solution_files', drupal_render($form['solution_files']));
		/*************************************************************************************/
	} //$solution_list_default_value != 0
	else
	{
		$commands[] = ajax_command_html('#ajax_selected_lab_experiment_solution_action', '');
		$commands[] = ajax_command_html('#ajax_download_experiment_solution', '');
		$commands[] = ajax_command_html('#ajax_edit_experiment_solution', '');
		$commands[] = ajax_command_html('#ajax_solution_files', '');
	}
	return array(
		'#type' => 'ajax',
		'#commands' => $commands
	);
}
/************************************************************/
function lab_migration_bulk_approval_form_validate($form, &$form_state)
{
	if (($form_state['values']['lab_actions'] == 0) && ($form_state['values']['lab_experiment_actions'] == 0) && ($form_state['values']['lab_experiment_solution_actions'] == 0))
		//var_dump($form_state['values']);die;
		drupal_set_message("1Wrong selection of action. Please select any one action", 'error');
	return;
	if (($form_state['values']['lab_actions'] != 0) && ($form_state['values']['lab_experiment_actions'] != 0) && ($form_state['values']['lab_experiment_solution_actions'] != 0))
		var_dump("2");die;
		drupal_set_message("Wrong selection of action. Please select any one action", 'error');
	return;
	if (($form_state['values']['lab_actions'] != 0) && ($form_state['values']['lab_experiment_actions'] != 0) && ($form_state['values']['lab_experiment_solution_actions'] == 0))
		var_dump("3");die;
		drupal_set_message("Wrong selection of action. Please select any one action", 'error');
	return;
	if (($form_state['values']['lab_actions'] != 0) && ($form_state['values']['lab_experiment_actions'] == 0) && ($form_state['values']['lab_experiment_solution_actions'] != 0))
		var_dump("4");die;
		drupal_set_message("Wrong selection of action. Please select any one action", 'error');
	return;
	if (($form_state['values']['lab_actions'] == 0) && ($form_state['values']['lab_experiment_actions'] != 0) && ($form_state['values']['lab_experiment_solution_actions'] != 0))
		var_dump("5");die;
		drupal_set_message("Wrong selection of action. Please select any one action", 'error');
	return;
}
function lab_migration_bulk_approval_form_submit($form, &$form_state)
{
	global $user;
	$root_path = lab_migration_path();
	var_dump($form_state['values']); 
	if ($form_state['clicked_button']['#value'] == 'Submit')
	{
		if ($form_state['values']['lab'])
			lab_migration_del_lab_pdf($form_state['values']['lab']);
		if (user_access('lab migration bulk manage code'))
		{
			if (($form_state['values']['lab_actions'] == 1) && ($form_state['values']['lab_experiment_actions'] == 0) && ($form_state['values']['lab_experiment_solution_actions'] == 0))
			{
				/* approving entire lab */
				//   $experiment_q = db_query("SELECT * FROM {lab_migration_experiment} WHERE proposal_id = %d", $form_state['values']['lab']);
				$query = db_select('lab_migration_experiment');
				$query->fields('lab_migration_experiment');
				$query->condition('proposal_id', $form_state['values']['lab']);
				$experiment_q = $query->execute();
				while ($experiment_data = $experiment_q->fetchObject())
				{
					//  db_query("UPDATE {lab_migration_solution} SET approval_status = 1, approver_uid = %d WHERE experiment_id = %d AND approval_status = 0", $user->uid, $experiment_data->id);
					db_query("UPDATE {lab_migration_solution} SET approval_status = 1, approver_uid = :approver_uid WHERE experiment_id = :experiment_id AND approval_status = 0", array(
						':approver_uid' => $user->uid,
						':experiment_id' => $experiment_data->id
					));
				} //$experiment_data = $experiment_q->fetchObject()
				db_update('lab_migration_lock')
					->fields(array(
						'code_interface' => 0,
						'journal_interface' => 1,
					))
					->condition('proposal_id', $form_state['values']['lab'])
					->execute();
				drupal_set_message(t('Approved Entire Lab.'), 'status');
				/* email */
				$email_subject = t('Your uploaded solutions have been approved');
				$email_body = array(
					0 => t('Your all the uploaded solutions for the Lab have been approved.')
				);
			} //($form_state['values']['lab_actions'] == 1) && ($form_state['values']['lab_experiment_actions'] == 0) && ($form_state['values']['lab_experiment_solution_actions'] == 0)
			elseif (($form_state['values']['lab_actions'] == 2) && ($form_state['values']['lab_experiment_actions'] == 0) && ($form_state['values']['lab_experiment_solution_actions'] == 0))
			{
				/* pending review entire lab */
				//$experiment_q = db_query("SELECT * FROM {lab_migration_experiment} WHERE proposal_id = %d", $form_state['values']['lab']);
				$experiment_q = db_query("SELECT * FROM {lab_migration_experiment} WHERE proposal_id = :proposal_id", array(
					':proposal_id' => $form_state['values']['lab']
				));
				while ($experiment_data = $experiment_q->fetchObject())
				{
					//db_query("UPDATE {lab_migration_solution} SET approval_status = 0 WHERE experiment_id = %d", $experiment_data->id);
					db_query("UPDATE {lab_migration_solution} SET approval_status = 0 WHERE experiment_id = :experiment_id", array(
						":experiment_id" => $experiment_data->id
					));
				} //$experiment_data = $experiment_q->fetchObject()
				drupal_set_message(t('Pending Review Entire Lab.'), 'status');
				/* email */
				$email_subject = t('Your uploaded solutions have been marked as pending');
				$email_body = array(
					0 => t('Your all the uploaded solutions for the Lab have been marked as pending to be review. You will be able to see the solutions after they have been approved by one of our reviewers.')
				);
			} //($form_state['values']['lab_actions'] == 2) && ($form_state['values']['lab_experiment_actions'] == 0) && ($form_state['values']['lab_experiment_solution_actions'] == 0)
			elseif (($form_state['values']['lab_actions'] == 3) && ($form_state['values']['lab_experiment_actions'] == 0) && ($form_state['values']['lab_experiment_solution_actions'] == 0))
			{
				if (!user_access('lab migration bulk delete code'))
				{
					drupal_set_message(t('You do not have permission to Bulk Dis-Approved and Deleted Entire Lab.'), 'error');
					return;
				} //!user_access('lab migration bulk delete code')
				if (lab_migration_delete_lab($form_state['values']['lab']))
				{
					drupal_set_message(t('Dis-Approved and Deleted Entire Lab.'), 'status');
				} //lab_migration_delete_lab($form_state['values']['lab'])
				else
				{
					drupal_set_message(t('Error Dis-Approving and Deleting Entire Lab.'), 'error');
				}
				/* email */
				$email_subject = t('Your uploaded solutions have been marked as dis-approved');
				$email_body = array(
					0 => t('Your all the uploaded solutions for the whole Lab have been marked as dis-approved.

Reason for dis-approval:

' . $form_state['values']['message'])
				);
			} //($form_state['values']['lab_actions'] == 3) && ($form_state['values']['lab_experiment_actions'] == 0) && ($form_state['values']['lab_experiment_solution_actions'] == 0)
			elseif (($form_state['values']['lab_actions'] == 4) && ($form_state['values']['lab_experiment_actions'] == 0) && ($form_state['values']['lab_experiment_solution_actions'] == 0))
			{//var_dump("")
				if (!user_access('lab migration bulk delete code'))
				{
					drupal_set_message(t('You do not have permission to Bulk Delete Entire Lab Including Proposal.'), 'error');
					return;
				} //!user_access('lab migration bulk delete code')
				/* check if dependency files are present */
				$dep_q = db_query("SELECT * FROM {lab_migration_dependency_files} WHERE proposal_id = :proposal_id", array(
					":proposal_id" => $form_state['values']['lab']
				));
				if ($dep_data = $dep_q->fetchObject())
				{
					drupal_set_message(t("Cannot delete lab since it has dependency files that can be used by others. First delete the dependency files before deleting the lab."), 'error');
					return;
				} //$dep_data = $dep_q->fetchObject()
				if (lab_migration_delete_lab($form_state['values']['lab']))
				{
					drupal_set_message(t('Dis-Approved and Deleted Entire Lab solutions.'), 'status');
					$dir_path = $root_path . $form_state['values']['lab'];
					if (is_dir($dir_path))
					{
						$res = rmdir($dir_path);
						if (!$res)
						{
							drupal_set_message(t("Cannot delete Lab directory : " . $dir_path . ". Please contact administrator."), 'error');
							return;
						} //!$res
					} //is_dir($dir_path)
					else
					{
						drupal_set_message(t("Lab directory not present : " . $dir_path . ". Skipping deleting lab directory."), 'status');
					}
					/* deleting full proposal */
					//$proposal_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE id = %d", $form_state['values']['lab']);
					$proposal_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE id = :id", array(
						":id" => $form_state['values']['lab']
					));
					$proposal_data = $proposal_q->fetchObject();
					$proposal_id = $proposal_data->id;
					db_query("DELETE FROM {lab_migration_experiment} WHERE proposal_id = :proposal_id", array(
						":proposal_id" => $proposal_id
					));
					db_query("DELETE FROM {lab_migration_proposal} WHERE id = :id", array(
						":id" => $proposal_id
					));
					drupal_set_message(t('Deleted Lab Proposal.'), 'status');
					/* email */
					$email_subject = t('Your uploaded solutions including the Lab proposal have been deleted');
					$email_body = array(
						0 => t('Your all the uploaded solutions including the Lab proposal have been deleted permanently.')
					);
				} //lab_migration_delete_lab($form_state['values']['lab'])
			
			else
			{
				drupal_set_message(t('Error Dis-Approving and Deleting Entire Lab.'), 'error');
			}
			} //($form_state['values']['lab_actions'] == 4) && ($form_state['values']['lab_experiment_actions'] == 0) && ($form_state['values']['lab_experiment_solution_actions'] == 0)
			elseif (($form_state['values']['lab_actions'] == 5) && ($form_state['values']['lab_experiment_actions'] == 0) && ($form_state['values']['lab_experiment_solution_actions'] == 0)) {
				//var_dump("Please reach here.");die;	
				db_update('lab_migration_lock')
					->fields(array(
						'code_interface' => 1,
						'journal_interface' => 0, 
					))
					->condition('proposal_id', $form_state['values']['lab'])
					->execute();
				drupal_set_message(t("Code submission interface reopened."), 'status');
			}
			elseif (($form_state['values']['lab_actions'] == 0) && ($form_state['values']['lab_experiment_actions'] == 1) && ($form_state['values']['lab_experiment_solution_actions'] == 0))
			{
				db_query("UPDATE {lab_migration_solution} SET approval_status = 1, approver_uid = :approver_uid WHERE experiment_id = :experiment_id AND approval_status = 0", array(
					":approver_uid" => $user->uid,
					":experiment_id" => $form_state['values']['lab_experiment_list']
				));
				drupal_set_message(t('Approved Entire Experiment.'), 'status');
				/* email */
				$email_subject = t('Your uploaded solutions have been approved');
				$email_body = array(
					0 => t('Your all the uploaded solutions for the experiment have been approved.')
				);
			} //($form_state['values']['lab_actions'] == 0) && ($form_state['values']['lab_experiment_actions'] == 1) && ($form_state['values']['lab_experiment_solution_actions'] == 0)
			elseif (($form_state['values']['lab_actions'] == 0) && ($form_state['values']['lab_experiment_actions'] == 2) && ($form_state['values']['lab_experiment_solution_actions'] == 0))
			{
				db_query("UPDATE {lab_migration_solution} SET approval_status = 0 WHERE experiment_id = :experiment_id", array(
					":experiment_id" => $form_state['values']['lab_experiment_list']
				));
				drupal_set_message(t('Entire Experiment marked as Pending Review.'), 'status');
				/* email */
				$email_subject = t('Your uploaded solutions have been marked as pending');
				$email_body = array(
					0 => t('Your all the uploaded solutions for the experiment have been marked as pending to be review.')
				);
			} //($form_state['values']['lab_actions'] == 0) && ($form_state['values']['lab_experiment_actions'] == 2) && ($form_state['values']['lab_experiment_solution_actions'] == 0)
			elseif (($form_state['values']['lab_actions'] == 1) && ($form_state['values']['lab_experiment_actions'] == 3) && ($form_state['values']['lab_experiment_solution_actions'] == 0))
			{
				if (!user_access('lab migration bulk delete code'))
				{
					drupal_set_message(t('You do not have permission to Bulk Dis-Approved and Deleted Entire Experiment.'), 'error');
					return;
				} //!user_access('lab migration bulk delete code')
				if (lab_migration_delete_experiment($form_state['values']['lab_experiment_list']))
				{
					drupal_set_message(t('Dis-Approved and Deleted Entire Experiment.'), 'status');
				} //lab_migration_delete_experiment($form_state['values']['lab_experiment_list'])
				else
				{
					drupal_set_message(t('Error Dis-Approving and Deleting Entire Experiment.'), 'error');
				}
				/* email */
				$email_subject = t('Your uploaded solutions have been marked as dis-approved');
				$email_body = array(
					0 => t('Your uploaded solutions for the entire experiment have been marked as dis-approved.

Reason for dis-approval:

' . $form_state['values']['message'])
				);
			} //($form_state['values']['lab_actions'] == 1) && ($form_state['values']['lab_experiment_actions'] == 3) && ($form_state['values']['lab_experiment_solution_actions'] == 0)
			elseif (($form_state['values']['lab_actions'] == 0) && ($form_state['values']['lab_experiment_actions'] == 0) && ($form_state['values']['lab_experiment_solution_actions'] == 1))
			{
				db_query("UPDATE {lab_migration_solution} SET approval_status = 1, approver_uid = :approver_uid WHERE id = :id", array(
					":approver_uid" => $user->uid,
					":id" => $form_state['values']['solution']
				));
				drupal_set_message(t('Solution approved.'), 'status');
				/* email */
				$email_subject = t('Your uploaded solution has been approved');
				$email_body = array(
					0 => t('Your uploaded solution has been approved.')
				);
			} //($form_state['values']['lab_actions'] == 0) && ($form_state['values']['lab_experiment_actions'] == 0) && ($form_state['values']['lab_experiment_solution_actions'] == 1)
			elseif (($form_state['values']['lab_actions'] == 0) && ($form_state['values']['lab_experiment_actions'] == 0) && ($form_state['values']['lab_experiment_solution_actions'] == 2))
			{
				db_query("UPDATE {lab_migration_solution} SET approval_status = 0 WHERE id = :id", array(
					":id" => $form_state['values']['lab_solution_list']
				));
				drupal_set_message(t('Solution marked as Pending Review.'), 'status');
				/* email */
				$email_subject = t('Your uploaded solution has been marked as pending');
				$email_body = array(
					0 => t('Your uploaded solution has been marked as pending to be review.')
				);
			} //($form_state['values']['lab_actions'] == 0) && ($form_state['values']['lab_experiment_actions'] == 0) && ($form_state['values']['lab_experiment_solution_actions'] == 2)
			elseif (($form_state['values']['lab_actions'] == 0) && ($form_state['values']['lab_experiment_actions'] == 0) && ($form_state['values']['lab_experiment_solution_actions'] == 3))
			{
				if (lab_migration_delete_solution($form_state['values']['lab_solution_list']))
				{
					drupal_set_message(t('Solution Dis-Approved and Deleted.'), 'status');
				} //lab_migration_delete_solution($form_state['values']['lab_solution_list'])
				else
				{
					drupal_set_message(t('Error Dis-Approving and Deleting Solution.'), 'error');
				}
				/* email */
				$email_subject = t('Your uploaded solution has been marked as dis-approved');
				$email_body = array(
					0 => t('Your uploaded solution has been marked as dis-approved.

Reason for dis-approval:

' . $form_state['values']['message'])
				);
			} //($form_state['values']['lab_actions'] == 0) && ($form_state['values']['lab_experiment_actions'] == 0) && ($form_state['values']['lab_experiment_solution_actions'] == 3)
			/****** sending email when everything done ******/
			if ($email_subject)
			{
				/*$email_to = variable_get('lab_migration_emails', '') . ', ' . $user->mail; */
				// $email_to = "contact@scilab.in" . ', ' . $user->mail;
				$email_to = "prashantsinalkar@gmail.com";
				$param['standard']['subject'] = $email_subject;
				$param['standard']['body'] = $email_body;
				if (!drupal_mail('lab_migration', 'standard', $email_to, language_default(), $param, variable_get('lab_migration_from_email', NULL), TRUE))
					drupal_set_message('Error sending email message.', 'error');
			} //$email_subject
		} //user_access('lab migration bulk manage code')
		else
		{
			drupal_set_message(t('You do not have permission to bulk manage code.'), 'error');
		}
	} //$form_state['clicked_button']['#value'] == 'Submit'
}
/************************************************************/
function _bulk_list_of_labs()
{
	$lab_titles = array(
		'0' => 'Please select...'
	);
	//$lab_titles_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE solution_display = 1 ORDER BY lab_title ASC");
	$query = db_select('lab_migration_proposal');
	$query->fields('lab_migration_proposal');
	$query->condition('approval_status', 1);
	$query->orderBy('lab_title', 'ASC');
	$lab_titles_q = $query->execute();
	while ($lab_titles_data = $lab_titles_q->fetchObject())
	{
		$lab_titles[$lab_titles_data->id] = $lab_titles_data->lab_title . ' (Proposed by ' . $lab_titles_data->name . ')';
	} //$lab_titles_data = $lab_titles_q->fetchObject()
	return $lab_titles;
}
function _ajax_bulk_get_experiment_list($lab_default_value = '')
{
	$experiments = array(
		'0' => 'Please select...'
	);
	//$experiments_q = db_query("SELECT * FROM {lab_migration_experiment} WHERE proposal_id = %d ORDER BY number ASC", $proposal_id);
	$query = db_select('lab_migration_experiment');
	$query->fields('lab_migration_experiment');
	$query->condition('proposal_id', $lab_default_value);
	$query->orderBy('number', 'ASC');
	$experiments_q = $query->execute();
	while ($experiments_data = $experiments_q->fetchObject())
	{
		$experiments[$experiments_data->id] = $experiments_data->number . '. ' . $experiments_data->title;
	} //$experiments_data = $experiments_q->fetchObject()
	return $experiments;
}
function _bulk_list_lab_actions()
{
	$lab_actions = array(
		0 => 'Please select...'
	);
	$lab_actions[1] = 'Approve Entire Lab (Opens Journal Upload interface.)';
	$lab_actions[2] = 'Pending Review Entire Lab';
	$lab_actions[3] = 'Dis-Approve Entire Lab (This will delete all the solutions in the lab)';
	$lab_actions[4] = 'Delete Entire Lab Including Proposal';
	$lab_actions[5] = 'Reopen Code submission interface.';
	return $lab_actions;
}
function _ajax_bulk_get_solution_list($lab_experiment_list = '')
{
	$solutions = array(
		0 => 'Please select...'
	);
	// $solutions_q = db_query("SELECT * FROM {lab_migration_solution} WHERE experiment_id = %d ORDER BY
	//  CAST(SUBSTRING_INDEX(code_number, '.', 1) AS BINARY) ASC,
	//   CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(code_number , '.', 2), '.', -1) AS UNSIGNED) ASC,
	//  CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(code_number , '.', -1), '.', 1) AS UNSIGNED) ASC", $experiment_id);
	$query = db_select('lab_migration_solution');
	$query->fields('lab_migration_solution');
	$query->condition('experiment_id', $lab_experiment_list);
	//$query->orderBy("CAST(SUBSTRING_INDEX(code_number, '.', 1) AS BINARY", "ASC");
	// $query->orderBy("CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(code_number , '.', 2), '.', -1) AS UNSIGNED", "ASC");
	// $query->orderBy("CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(code_number , '.', -1), '.', 1) AS UNSIGNED", "ASC");
	$solutions_q = $query->execute();
	while ($solutions_data = $solutions_q->fetchObject())
	{
		$solutions[$solutions_data->id] = $solutions_data->code_number . ' (' . $solutions_data->caption . ')';
	} //$solutions_data = $solutions_q->fetchObject()
	return $solutions;
}
function _bulk_list_experiment_actions()
{
	$lab_experiment_actions = array(
		0 => 'Please select...'
	);
	$lab_experiment_actions[1] = 'Approve Entire Experiment';
	$lab_experiment_actions[2] = 'Pending Review Entire Experiment';
	$lab_experiment_actions[3] = 'Dis-Approve Entire Experiment (This will delete all the solutions in the experiment)';
	return $lab_experiment_actions;
}
function _bulk_list_solution_actions()
{
	$lab_solution_actions = array(
		0 => 'Please select...'
	);
	$lab_solution_actions[1] = 'Approve Entire Solution';
	$lab_solution_actions[2] = 'Pending Review Entire Solution';
	$lab_solution_actions[3] = 'Dis-approve Solution (This will delete the solution)';
	return $lab_solution_actions;
}
