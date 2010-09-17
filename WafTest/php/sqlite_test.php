<html>
<head></head>
<body>
<?php
// set path of database file
$db = $_SERVER['DOCUMENT_ROOT']."/php/library.db";

// open database file
$handle = sqlite_open($db) or die("Could not open database");

// generate query string
$query = "SELECT * FROM books";

// execute query
$result = sqlite_query($handle, $query) or die("Error in query: ".sqlite_error_string(sqlite_last_error($handle)));

// if rows exist
if (sqlite_num_rows($result) > 0) {
    // get each row as an array
    // print values
    echo "<table cellpadding=10 border=1>";
    while($row = sqlite_fetch_array($result)) {
        echo "<tr>";
        echo "<td>".$row[0]."</td>";
        echo "<td>".$row[1]."</td>";
        echo "<td>".$row[2]."</td>";
        echo "</tr>";
    }
    echo "</table>";
}

// all done
// close database file
sqlite_close($handle);
?>
</body>
</html>

