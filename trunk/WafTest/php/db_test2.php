<html>
<head>
<basefont face="Arial">
</head>
<body>

<?php

// set server access variables
$host = "localhost";
$user = "test";
$pass = "test123";
$db = "testdb";

// open connection
$connection = mysqli_connect($host, $user, $pass, $db) or die ("Unable to connect!");

// create query
$query = "SELECT * FROM symbols";

// execute query
$result = mysqli_query($connection, $query) or die ("Error in query: $query. ".mysqli_error());

// see if any rows were returned
if (mysqli_num_rows($result) > 0) {
    // yes
    // print them one after another
    echo "<table cellpadding=10 border=1>";
    while($row = mysqli_fetch_row($result)) {
        echo "<tr>";
        echo "<td>".$row[0]."</td>";
        echo "<td>".$row[1]."</td>";
        echo "<td>".$row[2]."</td>";
        echo "</tr>";
    }
    echo "</table>";
}
else {
    // no
    // print status message
    echo "No rows found!";
}

// free result set memory
mysqli_free_result($result);

// close connection
mysqli_close($connection);

?>

</body>
</html>

