<?php

// initialize a session
session_start();

// increment a session counter
$_SESSION['counter']++;
?>
<html>
    <body>
        <p>You have viewed this page <?=$_SESSION['counter']?> times</p>
        <p>The session id is  <?=session_id()?></p>
    </body>
</html>

