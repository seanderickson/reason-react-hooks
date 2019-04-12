
open Common

type formField = 
  | Name
  | Channels
  | Mag;

let getValue = event => ReactEvent.Form.target(event)##value;

// let lens = [
//   (`name, s=> s.name, (s, name) => {...s, name}),
//   (`channels, s=> s.channels, (s, channels) => {...s, channels})
// ];


[@react.component]
let make = (~initialState: microscopeState, ~handleSubmit )=> {

  let (state, setState) = React.useState(()=>initialState);

  let formSubmit = (event) => {
    ReactEvent.Form.preventDefault(event);
    handleSubmit(_=>state);
  };

  let updateValue = (field, event) => {
    let value = event->getValue;
    
    setState( _ => switch(field) {
        | Name => {...state, name: value} 
        | Channels => {...state, channels: value} 
        | Mag => {...state, magnification: value} 
      } 
    );
  };

  <form className="form" onSubmit=(e=>formSubmit(e)) >
    <label htmlFor="name">(str("Name"))</label>
    <input id="name" value={state.name}
      onChange=updateValue(Name)
    />
    <label htmlFor="channels">(str("Channels:"))</label>
    <input id="channels" value={state.channels} 
      onChange=updateValue(Channels)
    />
    <label htmlFor="mag" >(str("Magnification:"))</label>
    <input id="mag" value={state.magnification} 
      onChange=updateValue(Mag)
    />
    <button type_="submit" >(str("submit"))</button>
  </form>
}