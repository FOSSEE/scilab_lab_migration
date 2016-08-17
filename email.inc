<?php
/**
 * Implementation of hook_mail().
 */
function lab_migration_mail($key, &$message, $params)
{
	global $user;
	$language = $message['language'];
	//$language = user_preferred_language($user);
	switch ($key)
	{
		case 'solution_proposal_approved':
			/* initializing data */
			//$proposal_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE id = %d LIMIT 1", $params['solution_proposal_approved']['proposal_id']);
			$query = db_select('lab_migration_proposal');
			$query->fields('lab_migration_proposal');
			$query->condition('id', $params['solution_proposal_approved']['proposal_id']);
			$query->range(0, 1);
			$proposal_q = $query->execute();
			$proposal_data = $proposal_q->fetchObject();
			if ($proposal_data->solution_display == 1)
			{
				$solution_display = 'Yes';
			} //$proposal_data->solution_display == 1
			else
			{
				$solution_display = 'No';
			}
			// $experiment_q = db_query("SELECT * FROM {lab_migration_experiment} WHERE proposal_id = %d ORDER BY number", $params['solution_proposal_approved']['proposal_id'], 1);
			$query = db_select('lab_migration_experiment');
			$query->fields('lab_migration_experiment');
			$query->condition('proposal_id', $params['solution_proposal_approved']['proposal_id']);
			$query->orderBy('number', 'ASC');
			$experiment_q = $query->execute();
			$experiment_list = '
      	';
			while ($experiment_data = $experiment_q->fetchObject())
			{
				$experiment_list .= $experiment_data->number . ' ' . $experiment_data->title . '
      	';
			} //$experiment_data = $experiment_q->fetchObject()
			$user_data = user_load($params['solution_proposal_approved']['user_id']);
			$message['subject'] = t('[!site_name] Lab Migration Solution Proposal Approval', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				0 => t('
Dear !user_name,

Your following Lab migration solution proposal has been approved:

Full Name : ' . $proposal_data->solution_provider_name_title . ' ' . $proposal_data->solution_provider_name . '
Email : ' . $user_data->mail . '
Contact No. : ' . $proposal_data->solution_provider_contact_ph . '
Department/Branch : ' . $proposal_data->solution_provider_department . '
University/Institute : ' . $proposal_data->solution_provider_university . '

Display solution on website : ' . $solution_display . '

List of experiments : ' . $experiment_list . '

Please ensure that ALL the codes follow guidelines at http://www.scilab.in/coding-guidelines-lab-migration

Best Wishes,

!site_name', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'solution_proposal_disapproved':
			$user_data = user_load($params['solution_proposal_disapproved']['user_id']);
			$message['subject'] = t('[!site_name] Lab Migration Solution Disapproval', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

Your Lab migration solution proposal has been disapproved.

Reason: ' . $params['solution_proposal_disapproved']['message'] . '

Best Wishes,

!site_name', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'proposal_received':
			/* initializing data */
			// $proposal_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE id = %d LIMIT 1", $params['proposal_received']['proposal_id']);
			// $proposal_data = $proposal_q->fetchObject();
			$query = db_select('lab_migration_proposal');
			$query->fields('lab_migration_proposal');
			$query->condition('id', $params['proposal_received']['proposal_id']);
			$query->range(0, 1);
			$proposal_data = $query->execute()->fetchObject();
			if ($proposal_data->solution_display == 1)
			{
				$solution_display = 'Yes';
			} //$proposal_data->solution_display == 1
			else
			{
				$solution_display = 'No';
			}
			if ($proposal_data->solution_provider_uid == 0)
			{
				$solution_provider_user = 'Open';
			} //$proposal_data->solution_provider_uid == 0
			else if ($proposal_data->solution_provider_uid == $proposal_data->uid)
			{
				$solution_provider_user = 'Proposer';
			} //$proposal_data->solution_provider_uid == $proposal_data->uid
			else
			{
				$solution_provider_user = 'Unknown';
			}
			// $experiment_q = db_query("SELECT * FROM {lab_migration_experiment} WHERE proposal_id = %d ORDER BY number",
			//  $params['proposal_received']['proposal_id'], 1);
			$query = db_select('lab_migration_experiment');
			$query->fields('lab_migration_experiment');
			$query->condition('proposal_id', $params['proposal_received']['proposal_id']);
			$query->orderBy('number', 'ASC');
			$experiment_q = $query->execute();
			$experiment_list = '
      	';
			while ($experiment_data = $experiment_q->fetchObject())
			{
				$experiment_list .= $experiment_data->number . ' ' . $experiment_data->title . '
      	';
			} //$experiment_data = $experiment_q->fetchObject()
			$user_data = user_load($params['proposal_received']['user_id']);
			$message['subject'] = t('[!site_name] Your Lab migration proposal has been received', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

We have received your following Lab migration proposal:

Full Name : ' . $proposal_data->name_title . ' ' . $proposal_data->name . '
Email : ' . $user_data->mail . '
Contact No. : ' . $proposal_data->contact_ph . '
Department/Branch : ' . $proposal_data->department . '
University/Institute : ' . $proposal_data->university . '

Display solution on website : ' . $solution_display . '

Solution Provided By : ' . $solution_provider_user . '

List of experiments : ' . 

$experiment_list . '

Your proposal is under review and you will soon receive an email from us regarding the same.

Please ensure that ALL the codes follow guidelines at http://www.scilab.in/coding-guidelines-lab-migration

Best Wishes,

!site_name', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'proposal_disapproved':
			/* initializing data */
			// $proposal_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE id = %d LIMIT 1", $params['proposal_disapproved']['proposal_id']);
			//$proposal_data = $proposal_q->fetchObject();
			$query = db_select('lab_migration_proposal');
			$query->fields('lab_migration_proposal');
			$query->condition('id', $params['proposal_disapproved']['proposal_id']);
			$query->range(0, 1);
			$proposal_data = $query->execute()->fetchObject();
			if ($proposal_data->solution_display == 1)
			{
				$solution_display = 'Yes';
			} //$proposal_data->solution_display == 1
			else
			{
				$solution_display = 'No';
			}
			if ($proposal_data->solution_provider_uid == 0)
			{
				$solution_provider_user = 'Open';
			} //$proposal_data->solution_provider_uid == 0
			else if ($proposal_data->solution_provider_uid == $proposal_data->uid)
			{
				$solution_provider_user = 'Proposer';
			} //$proposal_data->solution_provider_uid == $proposal_data->uid
			else
			{
				$solution_provider_user = 'Unknown';
			}
			// $experiment_q = db_query("SELECT * FROM {lab_migration_experiment} WHERE proposal_id = %d ORDER BY number", $params['proposal_disapproved']['proposal_id'], 1);
			$query = db_select('lab_migration_experiment');
			$query->fields('lab_migration_experiment');
			$query->condition('proposal_id', $params['proposal_disapproved']['proposal_id']);
			$query->orderBy('number', 'ASC');
			$experiment_q = $query->execute();
			$experiment_list = '
      	';
			while ($experiment_data = $experiment_q->fetchObject())
			{
				$experiment_list .= $experiment_data->number . ' ' . $experiment_data->title . '
      	';
			} //$experiment_data = $experiment_q->fetchObject()
			$user_data = user_load($params['proposal_disapproved']['user_id']);
			$message['subject'] = t('[!site_name] Your Lab migration proposal has been disapproved', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

Your following Lab migration proposal has been disapproved:

Reason for disapproval: ' . $proposal_data->message . '

Full Name : ' . $proposal_data->name_title . ' ' . $proposal_data->name . '
Email : ' . $user_data->mail . '
Contact No. : ' . $proposal_data->contact_ph . '
Department/Branch : ' . $proposal_data->department . '
University/Institute : ' . $proposal_data->university . '

Display solution on website : ' . $solution_display . '

Solution Provided By : ' . $solution_provider_user . '

List of experiments : ' . $experiment_list . '

Please ensure that ALL the codes follow guidelines at http://www.scilab.in/coding-guidelines-lab-migration

Best Wishes,

!site_name', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'proposal_approved':
			/* initializing data */
			// $proposal_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE id = %d LIMIT 1", $params['proposal_approved']['proposal_id']);
			$query = db_select('lab_migration_proposal');
			$query->fields('lab_migration_proposal');
			$query->condition('id', $params['proposal_approved']['proposal_id']);
			$query->range(0, 1);
			$proposal_q = $query->execute();
			$proposal_data = $proposal_q->fetchObject();
			if ($proposal_data->solution_display == 1)
			{
				$solution_display = 'Yes';
			} //$proposal_data->solution_display == 1
			else
			{
				$solution_display = 'No';
			}
			if ($proposal_data->solution_provider_uid == 0)
			{
				$solution_provider_user = 'Open';
			} //$proposal_data->solution_provider_uid == 0
			else if ($proposal_data->solution_provider_uid == $proposal_data->uid)
			{
				$solution_provider_user = 'Proposer';
			} //$proposal_data->solution_provider_uid == $proposal_data->uid
			else
			{
				$solution_provider_user = 'Unknown';
			}
			//$experiment_q = db_query("SELECT * FROM {lab_migration_experiment} WHERE proposal_id = %d ORDER BY number", $params['proposal_approved']['proposal_id'], 1);
			$query = db_select('lab_migration_experiment');
			$query->fields('lab_migration_experiment');
			$query->condition('proposal_id', $params['proposal_approved']['proposal_id']);
			$query->orderBy('number', 'ASC');
			$experiment_q = $query->execute();
			$experiment_list = '
      	';
			while ($experiment_data = $experiment_q->fetchObject())
			{
				$experiment_list .= $experiment_data->number . ' ' . $experiment_data->title . '
      	';
			} //$experiment_data = $experiment_q->fetchObject()
			$user_data = user_load($params['proposal_approved']['user_id']);
			$message['subject'] = t('[!site_name] Your Lab migration proposal has been approved', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				0 => t('
Dear !user_name,

Your following Lab migration proposal has been approved:

Full Name : ' . $proposal_data->name_title . ' ' . $proposal_data->name . '
Email : ' . $user_data->mail . '
Contact No. : ' . $proposal_data->contact_ph . '
Department/Branch : ' . $proposal_data->department . '
University/Institute : ' . $proposal_data->university . '

Display solution on website : ' . $solution_display . '

Solution Provided By : ' . $solution_provider_user . '

List of experiments : ' . $experiment_list . '

Please ensure that ALL the codes follow guidelines at http://www.scilab.in/coding-guidelines-lab-migration

Best Wishes,

!site_name', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'proposal_completed':
			/* initializing data */
			//$proposal_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE id = %d LIMIT 1", $params['proposal_completed']['proposal_id']);
			$query = db_select('lab_migration_proposal');
			$query->fields('lab_migration_proposal');
			$query->condition('id', $params['proposal_completed']['proposal_id']);
			$query->range(0, 1);
			$proposal_q = $query->execute();
			$proposal_data = $proposal_q->fetchObject();
			if ($proposal_data->solution_display == 1)
			{
				$solution_display = 'Yes';
			} //$proposal_data->solution_display == 1
			else
			{
				$solution_display = 'No';
			}
			if ($proposal_data->solution_provider_uid == 0)
			{
				$solution_provider_user = 'Open';
			} //$proposal_data->solution_provider_uid == 0
			else if ($proposal_data->solution_provider_uid == $proposal_data->uid)
			{
				$solution_provider_user = 'Proposer';
			} //$proposal_data->solution_provider_uid == $proposal_data->uid
			else
			{
				$user_data = user_load($proposal_data->solution_provider_uid);
				if (!$user_data)
				{
					$solution_provider_user = 'Unknown';
				} //!$user_data
			}
			//$experiment_q = db_query("SELECT * FROM {lab_migration_experiment} WHERE proposal_id = %d ORDER BY number", $params['proposal_completed']['proposal_id'], 1);
			$query = db_select('lab_migration_experiment');
			$query->fields('lab_migration_experiment');
			$query->condition('proposal_id', $params['proposal_completed']['proposal_id']);
			$query->orderBy('number', 'ASC');
			$experiment_q = $query->execute();
			$experiment_list = '
      	';
			while ($experiment_data = $experiment_q->fetchObject())
			{
				$experiment_list .= $experiment_data->number . ' ' . $experiment_data->title . '
      	';
			} //$experiment_data = $experiment_q->fetchObject()
			$user_data = user_load($params['proposal_completed']['user_id']);
			$message['subject'] = t('[!site_name] Congratulations for completion of the Lab migration', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

Following Lab migration has been completed sucessfully :

Full Name : ' . $proposal_data->name_title . ' ' . $proposal_data->name . '
Email : ' . $user_data->mail . '
Contact No. : ' . $proposal_data->contact_ph . '
Department/Branch : ' . $proposal_data->department . '
University/Institute : ' . $proposal_data->university . '

Display solution on website : ' . $solution_display . '

Solution Provided By : ' . $solution_provider_user . '

List of experiments : ' . $experiment_list . '

Your Lab solution is now available at following link to download.

http://scilab.in/lab_migration_run/' . $proposal_data->id . '

Now you should be able to propose a new Lab migration...

Please ensure that ALL the codes follow guidelines at http://www.scilab.in/coding-guidelines-lab-migration

Best Wishes,

!site_name', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'solution_proposal_received':
			/* initializing data */
			//$proposal_q = db_query("SELECT * FROM {lab_migration_proposal} WHERE id = %d LIMIT 1", $params['solution_proposal_received']['proposal_id']);
			$query = db_select('lab_migration_proposal');
			$query->fields('lab_migration_proposal');
			$query->condition('id', $params['solution_proposal_received']['proposal_id']);
			$query->range(0, 1);
			$proposal_q = $query->execute();
			$proposal_data = $proposal_q->fetchObject();
			if ($proposal_data->solution_display == 1)
			{
				$solution_display = 'Yes';
			} //$proposal_data->solution_display == 1
			else
			{
				$solution_display = 'No';
			}
			//$experiment_q = db_query("SELECT * FROM {lab_migration_experiment} WHERE proposal_id = %d ORDER BY number", $params['solution_proposal_received']['proposal_id'], 1);
			$query = db_select('lab_migration_experiment');
			$query->fields('lab_migration_experiment');
			$query->condition('proposal_id', $params['solution_proposal_received']['proposal_id']);
			$query->orderBy('number', 'ASC');
			$experiment_q = $query->execute();
			$experiment_list = '
      	';
			while ($experiment_data = $experiment_q->fetchObject())
			{
				$experiment_list .= $experiment_data->number . ' ' . $experiment_data->title . '
      	';
			} //$experiment_data = $experiment_q->fetchObject()
			$user_data = user_load($params['solution_proposal_received']['user_id']);
			$message['subject'] = t('[!site_name] Your Lab migration solution proposal has been received', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

We have received your proposal for solution for the following lab:

Faculty name : '. $proposal_data->name_title . ' ' . $proposal_data->name . '
Lab : '. $proposal_data->lab_title .'
Department : '. $proposal_data->department .'
University/Institute : '. $proposal_data->university .'

The solution provider’s details are given below

Solution provider name: '. $proposal_data->solution_provider_name_title .' '. $proposal_data->solution_provider_name . '
Department: '. $proposal_data->solution_provider_department .'
University/Institute:

Display solution on website : ' . $solution_display . '

List of experiments : ' . 

$experiment_list . '

Your proposal is under review and you will soon receive an email from us regarding the same.

Please ensure that ALL the codes follow guidelines at http://www.scilab.in/coding-guidelines-lab-migration

Best Wishes,

Scilab Team', array(
					'language' => $language->language
				))
			);
			break;
		case 'solution_uploaded':
			// $solution_q = db_query("SELECT * FROM {lab_migration_solution} WHERE id = %d LIMIT 1", $params['solution_uploaded']['solution_id']);
			$query = db_select('lab_migration_solution');
			$query->fields('lab_migration_solution');
			$query->condition('id', $params['solution_uploaded']['solution_id']);
			$query->range(0, 1);
			$solution_q = $query->execute();
			$solution_data = $solution_q->fetchObject();
			$user_data = user_load($params['solution_uploaded']['user_id']);
			$message['subject'] = t('[!site_name] You have uploaded solution', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

You have uploaded the following solution:

Example number : ' . $solution_data->code_number . '
Caption : ' . $solution_data->caption . '

The solution is under review. You will be notified when it has been approved.

Best Wishes,

!site_name', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'solution_approved':
			//$solution_q = db_query("SELECT * FROM {lab_migration_solution} WHERE id = %d LIMIT 1", $params['solution_approved']['solution_id']);
			$query = db_select('lab_migration_solution');
			$query->fields('lab_migration_solution');
			$query->condition('id', $params['solution_approved']['solution_id']);
			$query->range(0, 1);
			$solution_q = $query->execute();
			$solution_data = $solution_q->fetchObject();
			$user_data = user_load($params['solution_approved']['user_id']);
			$message['subject'] = t('[!site_name] Your uploaded solution has been approved', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

Your following solution has been approved:

Solution number : ' . $solution_data->code_number . '
Caption : ' . $solution_data->caption . '

Please ensure that ALL the codes follow guidelines at http://www.scilab.in/coding-guidelines-lab-migration

Best Wishes,

!site_name', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'solution_disapproved':
			$user_data = user_load($params['solution_disapproved']['user_id']);
			$message['subject'] = t('[!site_name] Your uploaded solution has been disapproved', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

Your following solution has been disapproved:

Solution number : ' . $params['solution_disapproved']['solution_number'] . '
Caption : ' . $params['solution_disapproved']['solution_caption'] . '

Reason for dis-approval : ' . $params['solution_disapproved']['message'] . '

Please ensure that ALL the codes follow guidelines at http://www.scilab.in/coding-guidelines-lab-migration

Best Wishes,

!site_name', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'solution_deleted_user':
			$user_data = user_load($params['solution_deleted_user']['user_id']);
			$message['subject'] = t('[!site_name] User has deleted pending solution', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

Your following pending solution has been deleted :

Title of the Lab : ' . $params['solution_deleted_user']['lab_title'] . '
Title of the Experiment : ' . $params['solution_deleted_user']['experiment_title'] . '
Solution number : ' . $params['solution_deleted_user']['solution_number'] . '
Caption : ' . $params['solution_deleted_user']['solution_caption'] . '

Please ensure that ALL the codes follow guidelines at http://www.scilab.in/coding-guidelines-lab-migration

Best Wishes,

!site_name', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'dependency_uploaded':
			$user_data = user_load($params['dependency_uploaded']['user_id']);
			$dependency_files = implode(',', $params['dependency_uploaded']['dependency_names']);
			$message['subject'] = t('[!site_name] You have uploaded dependency file', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

You have uploaded following dependency files :
    ' . $dependency_files . '

Please ensure that ALL the codes follow guidelines at http://www.scilab.in/coding-guidelines-lab-migration

Best Wishes,

!site_name', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'standard':
			$message['subject'] = $params['standard']['subject'];
			$message['body'] = $params['standard']['body'];
			break;
	} //$key
}
