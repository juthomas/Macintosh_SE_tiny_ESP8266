var yellowTimer = null;
var whiteTimer = null;
var redTimer = null;
var blueTimer = null;

function sleep(ms) {
	return new Promise(resolve => setTimeout(resolve, ms));
  }

  function zeroPad(nr,base){
	var  len = (String(base).length - String(nr).length)+1;
	return len > 0? new Array(len).join('0')+nr : nr;
  }


async function startSound() {


	// console.log(yellowTimer)
	var audio;
	// await sleep(1000);
	audio = new Audio('Race_Start.mp3');
	audio.play();

	console.log("Hello");
	// audio = new Audio('3.mp3');
	// audio.play();
	// await sleep(1000);
	// audio = new Audio('2.mp3');
	// audio.play();
	// await sleep(1000);
	// audio = new Audio('1.mp3');
	// audio.play();
	// await sleep(1000);
	// audio = new Audio('c_est_parti.mp3');
	// audio.play();
	console.log("End Of sounds");
}


document.addEventListener("DOMContentLoaded", function (event) {
	// try
	// {
		var Socket = null;
		if (window.location.hostname != "")
			Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
		// var Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
	// }
	// catch (exception)
	// {
	// //   print(exception);
	// }

	var startBtn = document.getElementById("startBtn");
	var stopBtn = document.getElementById("stopBtn");
	var offBtn = document.getElementById("offBtn");
	var redBtn = document.getElementById("redBtn");
	var orangeBtn = document.getElementById("orangeBtn");
	var greenBtn = document.getElementById("greenBtn");
	var lapTimeOnBtn = document.getElementById("lapTimeOnBtn");
	var clearBtn = document.getElementById("clearBtn");
	
	var sensorUpdateInput = document.getElementById("sensorUpdateInput");
	var sensorUpdateBtn = document.getElementById("sensorUpdateBtn");
	var sensorRefinementInput = document.getElementById("sensorRefinementInput");
	var sensorRefinementBtn = document.getElementById("sensorRefinementBtn");
	var sensorMaxDistanceInput = document.getElementById("sensorMaxDistanceInput");
	var sensorMaxDistanceBtn = document.getElementById("sensorMaxDistanceBtn");

	var soundChk = document.getElementById("soundChk");


	var getYellowLapBtn = document.getElementById("getYellowLapBtn");
	var getWhiteLapBtn = document.getElementById("getWhiteLapBtn");
	var getRedLapBtn = document.getElementById("getRedLapBtn");
	var getBlueLapBtn = document.getElementById("getBlueLapBtn");




	var redOn = false;
	var orangeOn = false;
	var greenOn = false;
	var lapTimeOn = true;
	lapTimeOnBtn.style.backgroundColor = "green";

	document.onkeydown = function(event){
		switch(event.code) {
		  case "Digit1" : /* le code 32 correspond à la touche Espace */
		  	getYellowLapBtn.click();
		  break;
		  case "Digit2" : /* le code 32 correspond à la touche Ctrl */
		  getWhiteLapBtn.click();

		  break;
		  case "Digit3" : /* le code 32 correspond à la touche Ctrl */
		  getRedLapBtn.click();
		
		  break;
		case "Digit4" : /* le code 32 correspond à la touche Ctrl */
		getBlueLapBtn.click();
	  
		break;
		case "KeyS" : /* le code 32 correspond à la touche Ctrl */
		startBtn.click();
	  
		break;
		case "KeyF" : /* le code 32 correspond à la touche Ctrl */
		stopBtn.click();
	  
		break;
		  default :
			/* Si la touche n'est pas répertoriée dans le script, on affiche le code de cette touche pour pouvoir l'ajouter (utile seulement pendant le développement, pour connaître le code des touches */
			console.log(event.keyCode);
		  break;
		}
	  }


	startBtn.addEventListener('click', () => {
		if (soundChk.checked)
		{
			startSound();
		}
		yellowTimer = Date.now() + 2250; 
		whiteTimer = Date.now() + 2250;
		redTimer = Date.now() + 2250;
		blueTimer = Date.now() + 2250;
		Socket.send("[START]" + Math.floor(Date.now()/1000));
	})
	stopBtn.addEventListener('click', () => {
		yellowTimer = null;
		whiteTimer = null;
		redTimer = null;
		blueTimer = null;
		Socket.send("[STOP]");
	})
	offBtn.addEventListener('click', () => {
		Socket.send("[OFF]");
	})

	redBtn.addEventListener('click', () => {
		redOn = !redOn;
		if (redOn)
		{
			redBtn.style.backgroundColor = "red";
			Socket.send("[RED] ON");
		}
		else
		{
			redBtn.style.backgroundColor = "";
			Socket.send("[RED] OFF");
		}
	})
	orangeBtn.addEventListener('click', () => {
		orangeOn = !orangeOn;
		if (orangeOn)
		{
			orangeBtn.style.backgroundColor = "orange";
			Socket.send("[ORANGE] ON");
		}
		else
		{
			orangeBtn.style.backgroundColor = "";
			Socket.send("[ORANGE] OFF");
		}
	})
	greenBtn.addEventListener('click', () => {
		greenOn = !greenOn;
		if (greenOn)
		{
			greenBtn.style.backgroundColor = "green";
			Socket.send("[GREEN] ON");
		}
		else
		{
			greenBtn.style.backgroundColor = "";
			Socket.send("[GREEN] OFF");
		}
	})
	lapTimeOnBtn.addEventListener('click', () => {
		lapTimeOn = !lapTimeOn;
		if (lapTimeOn)
		{
			lapTimeOnBtn.style.backgroundColor = "green";
			Socket.send("[LAPTIME] ON");
		}
		else
		{
			lapTimeOnBtn.style.backgroundColor = "";
			Socket.send("[LAPTIME] OFF");
		}
	})
	clearBtn.addEventListener('click', () => {
		document.getElementById("rxConsole").value = "";
	})


	sensorUpdateInput.addEventListener("keyup", event => {
		if(event.key !== "Enter") return; // Use `.key` instead.
		sensorUpdateBtn.click(); // Things you want to do.
		event.preventDefault(); // No need to `return false;`.
	});

	sensorUpdateBtn.addEventListener('click', () => {
		if (sensorUpdateInput.value)
		{
			Socket.send("[SETSENSORUPDATE] " + sensorUpdateInput.value);
		}
	})

	sensorRefinementInput.addEventListener("keyup", event => {
		if(event.key !== "Enter") return; // Use `.key` instead.
		sensorRefinementBtn.click(); // Things you want to do.
		event.preventDefault(); // No need to `return false;`.
	});


	sensorRefinementBtn.addEventListener('click', () => {
		if (sensorRefinementInput.value)
		{
			Socket.send("[SETREFINEMENT] " + sensorRefinementInput.value);
		}
	})

	sensorMaxDistanceInput.addEventListener("keyup", event => {
		if(event.key !== "Enter") return; // Use `.key` instead.
		sensorMaxDistance.click(); // Things you want to do.
		event.preventDefault(); // No need to `return false;`.
	});

	sensorMaxDistanceBtn.addEventListener('click', () => {
		if (sensorMaxDistanceInput.value)
		{
			Socket.send("[SETSENSORMAXDISTANCE] " + sensorMaxDistanceInput.value);
		}

	})

	getYellowLapBtn.addEventListener('click', () => {
		if (yellowTimer != null)
		{
			let lapTime = Date.now() - yellowTimer;
			yellowTimer = Date.now();
			console.log("laptime:", lapTime );
			document.getElementById("rxConsole").value += ("[YELLOWLAP] " + zeroPad(Math.floor(lapTime / 60000) % 100, 10)+":"+ zeroPad(Math.floor(lapTime / 1000) % 60, 10) + ":" + zeroPad(lapTime % 1000, 100) + "\n") ;
			document.getElementById("rxConsole").scrollTop = document.getElementById("rxConsole").scrollHeight;
		}
	})

	getWhiteLapBtn.addEventListener('click', () => {
		if (whiteTimer != null)
		{
			let lapTime = Date.now() - whiteTimer;
			whiteTimer = Date.now();
			console.log("laptime:", lapTime );
			document.getElementById("rxConsole").value += ("[WHITELAP] " + zeroPad(Math.floor(lapTime / 60000) % 100, 10)+":"+ zeroPad(Math.floor(lapTime / 1000) % 60, 10) + ":" + zeroPad(lapTime % 1000, 100) + "\n") ;
			document.getElementById("rxConsole").scrollTop = document.getElementById("rxConsole").scrollHeight;
		}
	})
	getRedLapBtn.addEventListener('click', () => {
		if (redTimer != null)
		{
			let lapTime = Date.now() - redTimer;
			redTimer = Date.now();
			console.log("laptime:", lapTime );
			document.getElementById("rxConsole").value += ("[REDLAP] " + zeroPad(Math.floor(lapTime / 60000) % 100, 10)+":"+ zeroPad(Math.floor(lapTime / 1000) % 60, 10) + ":" + zeroPad(lapTime % 1000, 100) + "\n") ;
			document.getElementById("rxConsole").scrollTop = document.getElementById("rxConsole").scrollHeight;
		}
	})

	getBlueLapBtn.addEventListener('click', () => {
		if (blueTimer != null)
		{
			let lapTime = Date.now() - blueTimer;
			blueTimer = Date.now();
			console.log("laptime:", lapTime );
			document.getElementById("rxConsole").value += ("[BLUELAP] " + zeroPad(Math.floor(lapTime / 60000) % 100, 10)+":"+ zeroPad(Math.floor(lapTime / 1000) % 60, 10) + ":" + zeroPad(lapTime % 1000, 100) + "\n") ;
			document.getElementById("rxConsole").scrollTop = document.getElementById("rxConsole").scrollHeight;
		}
	})

	// setInterval(() => {
	// 	if (Socket)
	// 		Socket.send("test");
	// }, 1000);
	if (Socket)
	{

		Socket.addEventListener('message', function (event) {
			// console.log('Message from server ', event.data);

			if (event.data.startsWith("[LAPSOUND]"))
			{
				// await sleep(1000);
				if(soundChk.checked)
				{
					var audio;
					audio = new Audio('RaceCheckPoint.mp3');
					audio.play();
				}
			}
			else if (event.data.startsWith("[LAPTIME]"))
			{
				document.getElementById("rxConsole").value += (event.data);
				document.getElementById("rxConsole").scrollTop = document.getElementById("rxConsole").scrollHeight;
				if(soundChk.checked)
				{
					var audio;
					// await sleep(1000);
					audio = new Audio('RaceCheckPoint.mp3');
					audio.play();
				}
			}
			else if (event.data.startsWith("[RACETIME]"))
			{
				document.getElementById("rxConsole").value += (event.data);
				document.getElementById("rxConsole").scrollTop = document.getElementById("rxConsole").scrollHeight;
				if(soundChk.checked)
				{
					var audio;
					// await sleep(1000);
					audio = new Audio('Applause.mp3');
					audio.play();
				}
			}

			// document.getElementById("rxConsole").value += event.data;
			// document.getElementById("rxConsole").scrollTop = document.getElementById("rxConsole").scrollHeight;
		});
	}
})