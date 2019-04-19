
open Common

type formField = 
  | Name
  // | Channels
  | Mag;

let getValue = event => ReactEvent.Form.target(event)##value;

// let lens = [
//   (`name, s=> s.name, (s, name) => {...s, name}),
//   (`channels, s=> s.channels, (s, channels) => {...s, channels})
// ];


[@react.component]
let make = (~initialState: microscope, ~handleSubmit )=> {

  let (msState, setState) = React.useState(()=>initialState);
  let (channelCount, setChannelCount) = React.useState(()=>List.length(msState.channels));

  let updateValue = (field, event) => {
    
    // Note: must cache the value because setState will run async to event
    let value = event->getValue;

    setState( _ => switch(field) {
        | Name => { ...msState, name: value } 
        | Mag => { ...msState, magnification: value }
      } 
    );
  };

  let updateChannelCount = requestedChannels => {
    let currLen = List.length(msState.channels);
    if (requestedChannels < currLen) {
      Js.log2("reduce channels...", requestedChannels);
      msState;
    } else if (requestedChannels > currLen) {
      let channels = ref(msState.channels);
      for (i in currLen+1 to requestedChannels){
        channels := List.append(channels^, [{ ordinal: i, filter: "Test-" ++ string_of_int(i)}]);
      }
      let newState = {...msState, channels: channels^  };
      setState(_=>newState);
      newState;
      // Note: don't setState directly, as submit will need the new state as well
      // setState(_=>{...msState, channels: channels^  });
    } else {
      msState;
    }
  };

  let formSubmit = (event) => {
    ReactEvent.Form.preventDefault(event);
    let newState = updateChannelCount(channelCount);
    handleSubmit(_=>newState);
  };

  let setChannels = (event) => {
    ReactEvent.Form.preventDefault(event);
    // handleSubmit(_=>msState);
  };

  <div>
  <form className="data_list_form" onSubmit=(e=>formSubmit(e)) >
    <label htmlFor="name">(str("Name"))</label>
    <input id="name" value={msState.name}
      onChange=updateValue(Name)
    />
    <label htmlFor="channels">(str("Channels:"))</label>
    <input id="channels" value=string_of_int(channelCount) onChange=(e=>setChannelCount(e->getValue)) />
    <label htmlFor="mag" >(str("Magnification:"))</label>
    <input id="mag" value={msState.magnification} 
      onChange=updateValue(Mag)
    />
    <button type_="submit" >(str("submit"))</button>
  </form>
  <hr />
  <ChannelForm channels={msState.channels} handleSubmit=setChannels />
  </div>
}