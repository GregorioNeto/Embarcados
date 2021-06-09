<?php
	if($_POST["desligar"]){
		shell_exec("echo 0 > /usr/Projeto/status_project.txt");	
	}

	if($_POST["ligar"]){
		shell_exec("echo 1 > /usr/Projeto/status_project.txt");	
	}
	
	if($_POST["automatico"]){
		shell_exec("echo 2 > /usr/Projeto/status_project.txt");
			
	}
	
	if(isset($_POST["timerconfig"])){
		$timer = $_POST['timerconfig'];	
		$timer1= fopen("/usr/Projeto/status_timer.txt","w");
		fprintf($timer1, $timer);
		fclose($timer1);	
	}
?>

<br>
<html>
<head><title>Irrigador Automatico</title>
<link rel="stylesheet" type="text/css" href="buton.css">

</head>
<div class="h1">
	<center><h1> IRRIGADOR AUTOMÁTICO </h1></center>
</div>

<body class="center-form">
	<div class="center-form">
		<form method="post" action="#">
			<h3> Irrigador: </h3>
			<div class="btnCenter">
				<button type="submit" value="ON" class="btn btn-green"  name="ligar">ON</button>
				<button type="submit" value="OFF" class="btn btn-green" name="desligar">OFF</button>
			</div>
			<br>
			<br>
			
			<h3> Temporizador: </h3>
			<div class="btnCenter">
				<input placeholder="Segundos" type="number" name="timerconfig" id="timer">
				<br>
				<br>
				<button type="submit" value="Auto" class="btn btn-green" name="automatico">AUTO</button>
			</div>
		</form>
	</div>

</body>
</html>
