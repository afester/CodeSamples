/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

import {debugConsole} from "./tools";
import $ from "jquery";

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
