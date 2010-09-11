<?php
    $file = '/home/zhangjw/tmp/test.txt' or die('Could not open file!');
    $fh = fopen($file, 'r') or die('Could not open file!');
    $data = fread($fh, filesize($file)) or die('Could not read file');
    fclose($fh);
?>
<html>
    <body>
        <p><?php echo $data; ?></p?
    </body>
</html>

