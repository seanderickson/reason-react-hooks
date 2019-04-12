open Common;

type formField = 
  | ProjectName
  | PI
  | PrimaryIrb
  | LspIrb
  | ProtocolIo
  | ScientistFrom
  | PathologyCore
  | CoPi
  | ScientistConducting
  | Grant
  | Purpose
  | Organism
  | SampleSize
  | Cycles
  | Comments;

type action =
  | UpdateValue(formField, string);


// let component = ReasonReact.reducerComponent("ProjectForm");

let getValue = event => ReactEvent.Form.target(event)##value;

// let errors: Hashtbl.t(formField,string) = Hashtbl.create(20);
// let getError = (field:formField) =>
//   try (errors -> Hashtbl.find(field)) {
//     | Not_found => ""
//   };

[@react.component]
let make = (~initialState: projectState, ~handleSubmit)=> {

  let (state, setState) = React.useState(()=>initialState);

  let update = (formField, value, state) =>
    switch(formField) {
      | ProjectName => {...state, projectName: value}
      | PI => {...state, pi: value}
      | PrimaryIrb => {...state, primaryIrb: value}
      | LspIrb => { ...state, lspIrb: value}
      | ProtocolIo => {...state, protocolIo: value}
      | ScientistFrom => {...state, scientistFrom: value}
      | PathologyCore => {...state, pathologyCore: value}
      | CoPi => {...state, 
          coPi: switch(value) {
            | "" => None
            | _ => Some(value)
          }}
      | ScientistConducting => {...state, scientistConducting: value}
      | Grant => {...state, grant: value}
      | Purpose => {...state, purpose: value}
      | Organism => {...state, organism: value}
      | SampleSize => {...state, 
        sampleSize: switch(value) {
          | "" => None
          | _ => Some(int_of_string(value))
        }}
      | Cycles => {...state, cycles: value}
      | Comments => {...state, comments: value}
    };

  let updateValue = (field, event) => {
    let value = event->getValue;
    setState( _ => update(field, value, state));
  };

  let formSubmit = (event) => {
    Js.log2("formSubmit...", state);
    ReactEvent.Form.preventDefault(event);
    handleSubmit(xstate=>state);
  };

  <form className="form" onSubmit=(e=>formSubmit(e)) >
    <label htmlFor="name">(str("Name"))</label>
    <input id="name" value={state.projectName} required=true
      onChange=updateValue(ProjectName)
    />
    // <div className="errors">(str(getError(ProjectName)))</div>
    <label htmlFor="pi">(str("PI:"))</label>
    <input id="pi" value={state.pi} 
      onChange=updateValue(PI) 
    />
    <label htmlFor="scientistFrom" >(str("Scientist samples received from:"))</label>
    <input id="scientistFrom" value={state.scientistFrom} 
      onChange=updateValue(ScientistFrom) 
    />
    <label htmlFor="primaryIrb" >(str("Primary IRB:"))</label>
    <input id="primaryIrb" value={state.primaryIrb} 
      onChange=updateValue(PrimaryIrb)
    />
    <label htmlFor="lspIrb" >(str("LSP IRB:"))</label>
    <input id="lspIrb" value={state.lspIrb} 
      onChange=updateValue(LspIrb) 
    />
    <label htmlFor="protocolIo" >(str("Protocol.io:"))</label>
    <input id="protocolIo" value={state.protocolIo} 
      onChange=updateValue(ProtocolIo) 
    />
    <label htmlFor="pathologyCore" >(str("Pathology Core:"))</label>
    <input id="pathologyCore" value={state.pathologyCore} 
      onChange=updateValue(PathologyCore) 
    />
    <label htmlFor="coPi" >(str("Internal LSP coPI/Primary contact:"))</label>
    <input id="coPi" value={
      switch(state.coPi) {
        | Some(value) => value
        | None => ""
      }} 
      onChange=updateValue(CoPi) 
    />
    <label htmlFor="scientistConducting" >(str("Scientist conducting CyCIF experiment:"))</label>
    <input id="scientistConducting" value={state.scientistConducting} 
      onChange=updateValue(ScientistConducting) 
    />
    <label htmlFor="grant" >(str("Grant:"))</label>
    <input id="grant" value={state.grant} 
      onChange=updateValue(Grant) 
    />
    <label htmlFor="purpose" >(str("Purpose:"))</label>
    <input id="purpose" value={state.purpose} 
      onChange=updateValue(Purpose) 
    />
    <label htmlFor="organism" >(str("Organism:"))</label>
    <input id="organism" value={state.organism} 
      onChange=updateValue(Organism) 
    />
    <label htmlFor="sampleSize" >(str("Sample Size:"))</label>
    <input id="sampleSize" value={
      switch(state.sampleSize) {
        | Some(value) => string_of_int(value)
        | None => ""
      }} 
      onChange=updateValue(SampleSize) 
    />
    <label htmlFor="cycles" >(str("Cycles:"))</label>
    <input id="cycles" value={state.cycles} 
      onChange=updateValue(Cycles) 
    />
    <label htmlFor="comments" >(str("Comments:"))</label>
    <input id="comments" value={state.comments} 
      onChange=updateValue(Comments)
    />
    <button type_="submit" >(str("submit"))</button>
  </form>
};

