/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

import {debugConsole, dumpObject} from "./tools";
import $ from "jquery";
import {setIn} from "immutable";

export function jquerySampleAction() {
  debugConsole.clear();
  debugConsole.writeln('HELLO jquery');
  debugConsole.writeln();

  // Note: jQuery returns a result even if the query did not find anything!
  const notFound = $('#notexist');
  debugConsole.writeln('RESULT OF NOTEXIST:' + notFound);
  debugConsole.writeln('NUMBER OF RESULTS : ' + notFound.length); // length is 0 if no result!
  debugConsole.writeln();

  // select by id
  const textArea = $('#_textArea');
  debugConsole.writeln('RESULT:' + textArea);
  debugConsole.writeln('NUMBER OF RESULTS : ' + textArea.length);
  debugConsole.writeln();

  // select by tag
  const buttons = $('button');
  debugConsole.writeln('RESULT:' + buttons);
  debugConsole.writeln('NUMBER OF RESULTS : ' + buttons.length);
  debugConsole.writeln();

  // select by class
  const fc = $('.float-content');
  debugConsole.writeln('RESULT:' + fc);
  debugConsole.writeln('NUMBER OF RESULTS : ' + fc.length);
  debugConsole.writeln();

  // Apply actions on the selected elements
  $('button').css('background-image', '-webkit-linear-gradient(#ff0000 0%, #00ff00 100%)');
  $('#properties').css('border', '2px solid yellow');
  $('#properties td').css('background-color', '#f6d2c2');
  $('#properties td:nth-child(3)').css('background-color', 'green');
}

function createRow(columns) {
  const colKeys = Object.keys(columns);

  const tr = document.createElement("tr");
  colKeys.forEach(key => {
    const cell1 = document.createElement("td");
    tr.appendChild(cell1);
    cell1.innerText = columns[key];
  });

  return tr;
}

function updateDOM(data) {
  dumpObject("RESULT", data);
  $("#firstname").text(data.firstname);
  $("#lastname").text(data.lastname);

  const tr = createRow(data);
  $("#table-data").append(tr);
}

export function promiseExample() {
  debugConsole.writeln("\nPromise REST Example");
  fetch("getPerson?id=1")
      .then(res => res.json())
      .then(updateDOM);
  debugConsole.writeln("REST Example DONE.");
}

function xyz(data) {
  debugConsole.writeln(data);
}

export function asyncExample() {
  debugConsole.writeln("\nAsync REST Example");

  const getPersonData = async () => {
    const res = await fetch("getPerson?id=2");
    // const { json } = res.json();   // Does not work
    const json = await res.json();  // Works

    updateDOM(json);
  };

  getPersonData();

  // Read the body and write it to the debug console
        // fetch("getNumbers?id=2")
        //     .then(res => res.text())
        //     .then(data => debugConsole.writeln(data));
  // fetch("getNumbers?id=2")
  //     .then(res => res.text())
  //     .then(xyz);   // NOTE: USING A METHOD REFERENCE DOES NOT WORK HERE!

  debugConsole.writeln("Async REST Example DONE.");
}


export function functionalExamples() {
  debugConsole.writeln("\nFunctional programming examples");

  debugConsole.writeln("\nfilter\n===========");
  const data = ['Peter', 'Michael', 'John', 'Joane'];
  const namesWithJ = data.filter(e => e.startsWith('J'));
  debugConsole.writeln(data + " => " + namesWithJ);

  debugConsole.writeln("\nmap\n===========");
  const newData = data.map((e, idx) => idx + e);
  debugConsole.writeln(data + " => " + newData);

  debugConsole.writeln("\nreduce\n===========");
  const reduced = data.reduce( (current, element, idx) => { current.unshift(element); return current; }, new Array());
  debugConsole.writeln(data + " => " + reduced);

  debugConsole.writeln("\nrecursion\n===========");
  const dan = {
    type: "person",
    data: {
      gender: "male",
      info: {
        id: 22,
        fullname: {
          first: "Dan",
          last: "Deacon"
        }
      }
    }
  };

  // braindead solution (executes a depth first search over the complete data structure)
//   function deepPick2(searchPath, obj, currentPath) {
// //  const deepPick2 = (searchPath, obj, currentPath) => {
//     for (let key of Object.keys(obj)) {
//       const value = obj[key];
//
//       let nextPath = "";
//       if (currentPath.length > 0) {
//         nextPath = currentPath + "." + key;
//       } else {
//         nextPath = key;
//       }
//
//       if (typeof(value) === "object") {
//         const result = deepPick2(searchPath, value, nextPath);
//         if (result !== undefined) {
//           return result;
//         }
//       } else {
//         if (nextPath === searchPath) {
//           return value;
//         }
//       }
//     }
//
//     return undefined;
//   }
//
//   const deepPick = (search, obj) => {
//     return deepPick2(search, obj, "");
//   }

  // Optimized approach from the book:
  // (traverses the given path until a value is found)
  const deepPick = (fields, object = {}) => {
    const [first, ...remaining] = fields.split(".");

    return remaining.length
      ? deepPick(remaining.join("."), object[first])
      : object[first];
  };

  let result = deepPick("type", dan); // "person"
  debugConsole.writeln("RESULT: " + result);
  result = deepPick("data.info.fullname.first", dan); // "Dan"
  debugConsole.writeln("RESULT: " + result);
  result = deepPick("data.info.fullname.first.second", dan); // "Dan"
  debugConsole.writeln("RESULT: " + result);
}


export function initializeClock() {
  const getCurrentTime = () => new Date();
  const serializeClockTime = date => ({
    hours: date.getHours(),
    minutes: date.getMinutes(),
    seconds: date.getSeconds()
  });

  const prependZero = key => clockTime => ({
    ...clockTime,
        [key]: clockTime[key] < 10 ? "0" + clockTime[key] : clockTime[key]
    });

  const compose = (...fns) => arg =>
    fns.reduce((composed, f) => f(composed), arg);


  // const doubleDigits = dateObj => {
  //   return compose(prependZero("hours"),
  //                  prependZero("minutes"),
  //                  prependZero("seconds"))(dateObj);
  // };
  const doubleDigits = dateObj =>
    compose(prependZero("hours"),
            prependZero("minutes"),
            prependZero("seconds"))(dateObj);
  const formatTime = dateObj => (dateObj.hours + ":" + dateObj.minutes + ":" + dateObj.seconds);

  const pipeline = compose(
      getCurrentTime,
      serializeClockTime,
      doubleDigits,
      formatTime);

  const spanElement = $("#clock-text");
  spanElement.text(pipeline());
  setInterval(() => {
    spanElement.text(pipeline());
  }, 1000);
}


function* primeNumbers() {
  const result = [1, 3, 5, 7, 11, 13, 17, 23];
  for (let x of result) {
    yield x;
  }
}

export function yieldExample() {
  debugConsole.writeln("Generator function Example\n=========================================");

  for (let x of primeNumbers()) {
    debugConsole.writeln(`Value: ${x}`);
  }
}
