<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Clocking System</title>
	<style>
	html, body{
		background-color: #F2F2F2;
		font-family: Arial;
		font-size: 1em;
	}
	table{
		border-spacing: 0;
		border-collapse: collapse;
		margin: 2;	
		align:center;
		text-align:center; 
		margin-left:auto; 
		margin-right:auto; 
	}
	th{
		padding: 12px;
		background-color: #FF837A;	
	}
	td{
		padding: 5px;
		background-color: #FFF;
		border: 1px solid #CCC;
	}	
	p{
		text-align:center; 
		text-decoration: underline;
	}
	</style>
</head>

<p>Felhasználók:</p>

<table width=850>
<tr>
<th width=150  bgcolor="#E6E6FA">Vezetéknév:</th>
<th width=150  bgcolor="#E6E6FA">Keresztnév:</th>
<th width=150  bgcolor="#E6E6FA">Kártya ID:</th>
<th width=150  bgcolor="#E6E6FA">Teljesítendő:</th>
<th width=200  bgcolor="#E6E6FA">Teljesített:</th>
<th width=50  bgcolor="#E6E6FA">Aktív:</th>
</tr>
</table>

<table width=850>
	<?php
	require_once('./dbaccess.php');
	if ($db->connect_errno) {
				echo "Failed to connect to MySQL: ("
				. $db->connect_errno . ") " . $db->connect_error;
			}
			
			$sql = 'SELECT * FROM users ORDER BY user_id';
			$result_db = $db->query($sql) or die('Error perform query!');
					
			while ($r = $result_db->fetch_object()) {				
					echo '<tr>';				            
					echo '<td width=150> ' . $r->first_name . ' </td>';
					echo '<td width=150> ' . $r->last_name . ' </td>';			
					echo '<td width=150> ' . $r->card_id . ' </td>';
					echo '<td width=150> ' . $r->total_hours . ' óra </td>';		
					
					// Kiszámoljuk a ledolgozott időt					
					$total_worked = new DateTime('0000-00-00 00:00');
					$temp = clone $total_worked;					
					$user3 = $r->user_id;					
					$sql3 = "SELECT * FROM events WHERE user_id=$user3";
					$result_db3 = $db->query($sql3) or die('Error perform query!');							
					
					while ($r3 = $result_db3->fetch_object()) {				
														
							if (($r3->clocked_out) != NULL) {
								$datetime1 = new DateTime($r3->clocked_in);
								$datetime2 = new DateTime($r3->clocked_out);								
								$interval = $datetime1->diff($datetime2);
								$total_worked->add($interval);
							}												
					}
					$total_worked_result = $temp->diff($total_worked)->format("%d nap %h óra %i perc");
					
					// Kiírjuk a ledolgozott időt
					echo '<td width=200> ' . $total_worked_result . ' </td>';
					
					// Kirakjuk a megfelelő ikont
					if (($r->is_clocked_in) == 0)
						echo '<td width=50> <img src="out.png"> </td>';			
					else if (($r->is_clocked_in) == 1)
						echo '<td width=50> <img src="in.png"> </td>';				
					
					echo '</tr>';
			}
	?>
</table>
<br><hr>

<p>Események:</p>

<table width=850>
<tr>
<th width=250  bgcolor="#E6E6FA">Dolgozó:</th>
<th width=200  bgcolor="#E6E6FA">Bejelentkezett:</th>
<th width=200  bgcolor="#E6E6FA">Kijelentkezett:</th>
<th width=200  bgcolor="#E6E6FA">Időtartam:</th>
</tr>
</table>

<table width=850>
	<?php	
	if ($db->connect_errno) {
				echo "Failed to connect to MySQL: ("
				. $db->connect_errno . ") " . $db->connect_error;
			}
			
			$sql = 'SELECT * FROM events ORDER BY clock_id DESC';
			$result_db = $db->query($sql) or die('Error perform query!');
					
			while ($r = $result_db->fetch_object()) {				
					
					// Az aktuális dolgozó nevének kiiratása az user_id alapján
					$user = $r->user_id;
					$sql2 = "SELECT first_name, last_name FROM users WHERE user_id=$user";
					$result2_db = $db->query($sql2) or die('Error perform query!');
					$r2 = $result2_db->fetch_object(); //$r2->first_name . ' ' . $r2->last_name tartalmazza a nevet
					
					// Ha nem jelentkezett ki, akkor még dolgozik
					if (($r->clocked_out) == NULL) {
						$duration = "dolgozik";
					}
					// Különben kiírjuk, hogy mennyit dolgozott
					else {	
						// A két dátum alapján kiszámolni a különbséget					
						$datetime1 = new DateTime($r->clocked_in);
						$datetime2 = new DateTime($r->clocked_out);
						$interval = $datetime1->diff($datetime2);
						
						// Ha kevesebbet dolgozott, mint 1 nap, akkor a napot elrejtjük						
						if ( $interval->format('%d') < 1 ) {
							$duration = $interval->format('%h óra %i perc');							
						}	
						// Különben kiírjuk napostól
						else
							$duration = $interval->format('%d nap %h óra %i perc');	
					}
										
					echo '<tr>';				            
					echo '<td width=250> ' . $r2->first_name . ' ' . $r2->last_name . '</td>';
					echo '<td width=200> ' . $r->clocked_in . ' </td>';			
					echo '<td width=200> ' . $r->clocked_out . ' </td>';
					echo '<td width=200> ' . $duration . ' </td>';		
					echo '</tr>';
				}
			$db->close(); 
	?>
</table>
<br><hr>
</body>
</html>