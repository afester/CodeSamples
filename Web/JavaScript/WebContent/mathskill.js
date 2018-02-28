/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */


'use strict';


var counter
var countDown
var expectedResult

function startCountdown(from) {
	countDown = from

	// set initial value
    document.getElementById("counterValue").innerHTML = countDown
    countDown--

	// Update the count down every 1 second
	counter = setInterval(function() {

      // Display the result in the element with id="demo"
	  document.getElementById("counterValue").innerHTML = countDown
	  countDown--

	  // If the count down is finished, write some text 
	  if (countDown < 0) {
	    clearInterval(counter);
	    document.getElementById("result").innerHTML = "Die Zeit ist leider abgelaufen";
    	document.getElementById("result").className = "incorrect";
        document.getElementById("checkResult").className = "hidden";
        document.getElementById("newExcercise").className = "visible";
	   }
	}, 1000);
}


function getRandomFactor1() {
   var numbers = [1, 2, 3, 4, 5, 10];
   var idx = Math.floor(Math.random() * numbers.length);
   return numbers[idx];
}

function getRandomFactor2() {
   var numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
   var idx = Math.floor(Math.random() * numbers.length);
   return numbers[idx];
}


function newExcercise() {
   var factor1 = getRandomFactor1();
   var factor2 = getRandomFactor2();
   var order = Math.floor(Math.random() * 2);

   if (order == 1) {
	   var temp = factor1
	   factor1 = factor2
	   factor2 = temp
   }

   expectedResult = factor1 * factor2;

   document.getElementById("aufgabe").innerHTML = factor1 + " &middot; " + factor2;
   document.getElementById("resultInput").focus();
   document.getElementById("resultInput").value = ""
   document.getElementById("result").innerHTML = ""

   document.getElementById("checkResult").className = "visible";
   document.getElementById("newExcercise").className = "hidden";

   startCountdown(10)
}


function checkSolution() {
	var enteredValue = document.getElementById("resultInput").value;
    clearInterval(counter);
    
    if (enteredValue == expectedResult) {
    	document.getElementById("result").innerHTML = "Prima, richtig!";
    	document.getElementById("result").className = "correct";
    } else {
    	document.getElementById("result").innerHTML = "Leider falsch ...!";
    	document.getElementById("result").className = "incorrect";
    }

    document.getElementById("checkResult").className = "hidden";
    document.getElementById("newExcercise").className = "visible";
}


function init() {
	const textbox = document.getElementById("resultInput");
	textbox.addEventListener("keypress", function onEvent(event) {
	    if (event.key === "Enter") {
	        document.getElementById("checkResult").click();
	    }
	});

	document.getElementById("checkResult").className = "hidden";
}
