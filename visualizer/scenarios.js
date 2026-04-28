// Scenario data from actual solver output
const SCENARIOS = [
  {
    name: "Equilateral Triangle",
    desc: "Three distance constraints (d=5) forming an equilateral triangle with P0 fixed at origin.",
    converged: true, iterations: 34, error: 7.424e-7,
    constraints: [
      { type: "distance", p: [0,1], val: 5 },
      { type: "distance", p: [1,2], val: 5 },
      { type: "distance", p: [2,0], val: 5 },
    ],
    initial: [ {x:0,y:0,fixed:true}, {x:6,y:0.5}, {x:2,y:4} ],
    final:   [ {x:0,y:0,fixed:true}, {x:4.9869,y:0.3612}, {x:2.1807,y:4.4994} ],
  },
  {
    name: "Rectangle (4×3)",
    desc: "Horizontal + Vertical + Distance constraints shaping a perfect rectangle.",
    converged: true, iterations: 34, error: 9.212e-7,
    constraints: [
      { type: "horizontal", p: [0,1] },
      { type: "horizontal", p: [3,2] },
      { type: "vertical",   p: [0,3] },
      { type: "vertical",   p: [1,2] },
      { type: "distance",   p: [0,1], val: 4 },
      { type: "distance",   p: [1,2], val: 3 },
      { type: "distance",   p: [2,3], val: 4 },
      { type: "distance",   p: [3,0], val: 3 },
    ],
    initial: [ {x:0,y:0,fixed:true}, {x:4.5,y:0.3}, {x:3.8,y:3.2}, {x:0.2,y:2.8} ],
    final:   [ {x:0,y:0,fixed:true}, {x:4,y:0},     {x:4,y:3},     {x:0,y:3} ],
  },
  {
    name: "Pendulum Arm",
    desc: "Fixed anchor at (0,10) with a distance constraint (d=7) to a free point.",
    converged: true, iterations: 21, error: 7.883e-7,
    constraints: [
      { type: "fixed", p: [0], target: {x:0,y:10} },
      { type: "distance", p: [0,1], val: 7 },
    ],
    initial: [ {x:0,y:10,fixed:true}, {x:3,y:5} ],
    final:   [ {x:0,y:10,fixed:true}, {x:3.6015,y:3.9975} ],
  },
  {
    name: "90° Angle",
    desc: "Angle constraint at vertex P1 enforcing 90° between arms, plus distance constraints.",
    converged: true, iterations: 1, error: 0,
    constraints: [
      { type: "angle", p: [0,1,2], val: 90 },
      { type: "distance", p: [1,0], val: 3 },
      { type: "distance", p: [1,2], val: 4 },
    ],
    initial: [ {x:3,y:0}, {x:0,y:0,fixed:true}, {x:0,y:4} ],
    final:   [ {x:3,y:0}, {x:0,y:0,fixed:true}, {x:0,y:4} ],
  },
  {
    name: "Coincident Merge",
    desc: "Two separate points merged to a single location via CoincidentConstraint.",
    converged: true, iterations: 24, error: 8.596e-7,
    constraints: [
      { type: "coincident", p: [0,1] },
    ],
    initial: [ {x:1,y:2}, {x:5,y:8} ],
    final:   [ {x:3,y:5}, {x:3,y:5} ],
  },
  {
    name: "Over-constrained ✗",
    desc: "Both points fixed 5 apart, but constraint demands d=3. Solver correctly fails.",
    converged: false, iterations: 1000, error: 2.0,
    constraints: [
      { type: "distance", p: [0,1], val: 3 },
    ],
    initial: [ {x:0,y:0,fixed:true}, {x:5,y:0,fixed:true} ],
    final:   [ {x:0,y:0,fixed:true}, {x:5,y:0,fixed:true} ],
  },
];
