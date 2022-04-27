/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

import Immutable from 'immutable';
import {debugConsole} from "./tools";

export function variablesAction() {
  let x=4;
  debugConsole.writeln("x:" + x);

  x=5;
  debugConsole.writeln("x:" + x);

  const y = 6;
  debugConsole.writeln("y:" + y);

  // y = 7;     // TypeError: Assignment to constant variable.

  const numbers = [1, 3, 5, 7, 11];
  debugConsole.writeln("numbers:" + numbers);

  // numbers = [2, 4, 6, 8, 10];     // TypeError: Assignment to constant variable.

  numbers[2] = 123;
  debugConsole.writeln("numbers:" + numbers);

  numbers.forEach(n => debugConsole.writeln("   n:" + n));

  const oddNumbers = Immutable.List([1, 3, 5, 7, 9, 11]);
  debugConsole.writeln("oddNumbers:" + oddNumbers);

  // oddNumbers = Immutable.List([1, 3, 5, 7, 9, 11, 13]);     // TypeError: Assignment to constant variable.
  oddNumbers[1] = 4;                                           // No error, but does not have any effect!
  debugConsole.writeln("oddNumbers:" + oddNumbers);
}


export function enhancedVariablesAction() {
  debugConsole.writeln("Destructuring objects");

  const address = {
    firstname: 'Peter',
    lastname: 'Shaw',
    city: 'Rocky Beach',
    age: 18,
    data: {
      a: 4,
      b: 5,
    }
  }

  const {firstname, lastname} = address;
  debugConsole.writeln(`${firstname} ${lastname}`);

  debugConsole.writeln("Destructuring arrays");
  const animals = ["Horse", "Mouse", "Cat"];
  debugConsole.writeln(animals);
  const [firstAnimal] = animals;
  debugConsole.writeln(firstAnimal);
  const [, secondAnimal] = animals;
  debugConsole.writeln(secondAnimal);

  debugConsole.writeln("Spread operator");
  const wildAnimals = ["Lion", "Tiger", "Leopard"];
  const allAnimals = [...animals, ...wildAnimals];
  debugConsole.writeln(allAnimals);

  function sumValues(...args) {
    let result = 0;
    args.forEach(v => result = result + v);
    return result;
  }
  debugConsole.writeln(sumValues(1, 2, 3, 4));

  debugConsole.writeln("\nfor-loops and Object methods\n------------------------------------");
  for (let x in address) {
    debugConsole.writeln(x);
  }
  debugConsole.writeln("-------");

  // for (let x of address) {   // NOT iterable!
  //   debugConsole.writeln(x);
  // }
  // debugConsole.writeln("-------");
  for (let x of Object.keys(address)) { // NOTE:Object.keys() returns an array, hence need to use "of" to get the array elements
                                        // ("in" would iterate over the array keys which is 0, 1, 2, ...)
    debugConsole.writeln(x);
  }
  debugConsole.writeln("-------");
  for (let x of Object.getOwnPropertyNames(address)) {
    debugConsole.writeln(x);
  }
  debugConsole.writeln("-------");
}

export function nonworking() {
  const string = "Restaurants in Hanalei";
  const urlFriendly = "";

  for (var i = 0; i < string.length; i++) {
    if (string[i] === " ") {
      urlFriendly += "-";
    } else {
      urlFriendly += string[i];
    }
  }

  console.log(urlFriendly); // "Restaurants-in-Hanalei"
}
