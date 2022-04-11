/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

// ????????? How to properly do this?
// import Immutable from './node_modules/immutable/dist/immutable.es.js';
// import Immutable from './immutable';
import Immutable from 'immutable';

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

// export {variablesAction}
