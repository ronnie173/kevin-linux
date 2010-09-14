<?php
    $username = $_POST['username'];
    $password = $_POST['password'];
    $return = $_POST['return'];

    if ($username == 'test' && $password == 'test123') {
        // redirect to return page with auth cookie
        setcookie("PassportKey", "123456", time() + 3600, 
            "/", ".indtab.com", TRUE);
        header("Location: $return");
    } else {
        header('Location: /php/phpinfo.php');
    }
?>
    