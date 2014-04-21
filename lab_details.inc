<?php                                                                           
    function lab_migration_completed_labs_all() {                               
        $page_content = "";                                                     
        $query = "SELECT * FROM {lab_migration_proposal} WHERE approval_status = 3";
        $result = db_query($query);                                             
                                                                                
        $page_content .= "<ol>";                                                
        while($row = db_fetch_object($result)) {                                
            $page_content .= "<li>";                                            
            $page_content .= l($row->university." ({$row->lab_title})", "lab_migration_run/".$row->id);
            $page_content .= "</li>";                                           
        }                                                                       
        $page_content .= "</ol>";                                               
        return $page_content;                                                   
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
