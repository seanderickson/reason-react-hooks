
type projectState = {
  projectName: string,
  pi: string,
  primaryIrb: string,
  lspIrb: string,
  protocolIo: string,
  scientistFrom: string,
  pathologyCore: string,
  coPi: option(string),
  scientistConducting: string,
  grant: string,
  purpose: string,
  organism: string,
  sampleSize: option(int),
  cycles: string,
  comments: string
};

type channel = {
  ordinal: int,
  filter: string,
};

type channels = list(channel);

type microscope = {
  name: string,
  channels,
  magnification: string,
};

type route = 
  | Project
  | Microscope;
  // | Samples
  // | Planned
  // | Actual;

// type routerState = {
//   route,
//   projectState,
//   microscopeState,
//   channels
// };

type modalAction = 
  Hide 
  | Show(string, (string)=>unit, (string)=>unit)
  | ModalCancel
  | ModalOk;

type modalState = {
  shown: bool,
  message: string,
  callBackOk: (string)=>unit,
  callBackCancel: (string)=>unit
};


// Create some microscopes (TODO: get from API)
let storedMicroscopes = [
  { 
    name: "MS1",
    magnification: "20x HD",
    channels:  [
      { ordinal: 1, filter: "DAPI"},
      { ordinal: 2, filter: "FITC"},
      { ordinal: 3, filter: "CY3"},
      { ordinal: 4, filter: "CY5"},
    ]
  },
  { 
    name: "MS2",
    magnification: "10x",
    channels:  [
      { ordinal: 1, filter: "ABC"},
      { ordinal: 2, filter: "DEF"},
      { ordinal: 3, filter: "CY1"},
      { ordinal: 4, filter: "CY2"},
      { ordinal: 5, filter: "CY3"},
    ]
  },
  { 
    name: "MS3",
    magnification: "30x HD",
    channels:  [
      { ordinal: 1, filter: "MEK1"},
      { ordinal: 2, filter: "DAPI"},
      { ordinal: 3, filter: "CY2"},
      { ordinal: 4, filter: "CY4"},
      { ordinal: 5, filter: "FIT1"},
      { ordinal: 6, filter: "FIT2"},
    ]
  },
  { 
    name: "MS4",
    magnification: "15x HD",
    channels:  [
      { ordinal: 1, filter: "DAPI"},
      { ordinal: 2, filter: "JKLM"},
      { ordinal: 3, filter: "CY5"},
      { ordinal: 4, filter: "CY6"},
      { ordinal: 5, filter: "CY7"},
      { ordinal: 6, filter: "CY8"},
      { ordinal: 7, filter: "CY9"},
      { ordinal: 8, filter: "FIT1"},
    ]
  },
];


let str = ReasonReact.string;

// Construct a microscopeMap, for convenience in the UI:
// as: Belt.Map(string, microscope, module StringComp)
// Use the convenience functor Belt.Map.String

module BMS = Belt.Map.String;

let msPairs = 
  storedMicroscopes 
  |> List.map(m => (m.name, m) )
  |> Array.of_list;

let microscopeMap = msPairs -> BMS.fromArray; 


// type context = Foo | Bar;
// type contextFun = (context) => context;
// let context = React.createContext(Foo);

// module ContextProvider = {
//   let make = context->React.Context.provider;

//   [@bs.obj]
//   external makeProps:
//     (
//       ~value: context,
//       ~children: React.element,
//       ~key: string=?,
//       unit,
//     ) =>
//     {
//       .
//       "value": context,
//       "children": React.element,
//     } =
//     "";
// };