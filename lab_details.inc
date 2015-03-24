<?php
    function lab_migration_completed_labs_all() {
        $page_content = "";
        $query = "SELECT * FROM {lab_migration_proposal} WHERE approval_status = 3";
        $result = db_query($query);
        $proposal_rows = array();
        $i=1;
            while($row = db_fetch_object($result)) {
                $approval_date=date("Y", $row->approval_date);
                $preference_rows[] = array($i, $row->university , l($row->lab_title,"lab_migration_run/".$row->id),$approval_date);
                $i++;
            }
        $preference_header = array('No','Institute', 'Lab', 'Year');
        $output = theme_table($preference_header, $preference_rows);
        return $output;
    }                                                                           

    function lab_migration_labs_progress_all() {
        $page_content = "";
        $query = "SELECT * FROM {lab_migration_proposal} WHERE approval_status = 1 and solution_status = 2";
        $result = db_query($query);
        $page_content .= "<ol>";
        while($row = db_fetch_object($result)) {
            $page_content .= "<li>";
            $page_content .= $row->university . " ({$row->lab_title})";
            $page_content .= "</li>";
        }
        $page_content .= "</ol>";
        return $page_content;
    }
?>