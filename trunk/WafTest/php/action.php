<html>
  <body>
    <p>User Agent is <?php echo $_SERVER['HTTP_USER_AGENT']; ?></p>
    <p>Name is <?php echo htmlspecialchars($_POST['name']); ?></p>
    <p>Age is <?php echo (int)$_POST['age']; ?>
  </body>
</html>

