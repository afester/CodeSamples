import Immutable from 'immutable';
import {debugConsole} from "./tools";


export function immutableAction() {
    debugConsole.writeln("Immutable examples");
    debugConsole.writeln("=====================");

    const immuMap = Immutable.Map({"a": 1, "b": 2});
    debugConsole.writeln("immuMap:" + immuMap);

    const immuMap2 = immuMap.set("c", 3)
    debugConsole.writeln("immuMap2:" + immuMap2);

    // Create a sample data structure
    const immuList1 = Immutable.fromJS([
        {"name": "x1", "data": 12},
        {"name": "x2", "data": 15},
        {"name": "x3", "data": 23},
    ]);

    debugConsole.writeln("immuList1:" + immuList1);
    const numEntries = immuList1.forEach(e => {

    });
    debugConsole.writeln("Processed: " + numEntries);

    const immuList2 = immuList1.withMutations(list1 => {
        list1.push(Immutable.Map({"name": "x4", "data": 45}));
        list1.push(Immutable.Map({"name": "x5", "data": 66}));
    });
    debugConsole.writeln("Result: " + immuList2);

    // Add a new name/value pair to each dict in the list
    const newList = immuList2.map(e =>
        e.set("newKey", "value")
    );
    debugConsole.writeln("Result: " + newList);

    // set some value for a given name
    const name = "x4";
    const newList2 = immuList2.map(e =>
        e.get("name") === name ? e.set("newKey", "value") : e
    );
    debugConsole.writeln("Result newList2: " + newList2);

    // For a given list of names, add an element to the corresponding map
    // const objectsToModify = Immutable.List(["x1", "x3", "x4"]);
}
