import {debugConsole} from "./tools";

// https://stackoverflow.com/questions/37325667/does-es6-module-importing-execute-the-code-inside-the-imported-file

// export { name1, name2, …, nameN };
// export { variable1 as name1, variable2 as name2, …, nameN };

// export default function (…) { … } // oder: class, function*
// export default function name1(…) { … } // oder: class, function*
// export { name1 as default, … };

/** Re-exports */
// export * from …;
// export { name1, name2, …, nameN } from …;
// export { import1 as name1, import2 as name2, …, nameN } from …;

function calculateSomething() {
    return Math.sqrt(2);
}

// export let name1, name2, …, nameN;         // oder: var
// export let name1 = …, name2 = …, …, nameN; // oder: var, const
export const someExport = calculateSomething();

// export default expression;
// The expression is evaluated when the symbol is imported for the first time!

// export default 60*60;

// Also a function call is possible!
export default debugConsole.writeln("DEFAULT EXPORT");
