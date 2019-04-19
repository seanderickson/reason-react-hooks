
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

  /**
    Add or remove channels based on the Microscope Form input field.
    
    NOTE: return the new state so that it may be submitted to the parent.
   */
  let updateChannelCount = newCount => {

    let currLen = List.length(msState.channels);
    if (newCount < currLen) {
      let newState = { 
        ...msState, 
        channels: msState.channels 
          |> ListLabels.fold_left(
            ~f=(newList,channel) => 
              List.length(newList) < newCount ? newList @ [channel] : newList, 
            ~init=[])
      };  
      setState(_=>newState);
      newState;
    } else if (newCount > currLen) {

      let maxOrdinal = msState.channels 
        |> List.fold_left(
          (max,channel) => channel.ordinal>max ? channel.ordinal : max, 0);

      // // Method 1: using mutable reference
      // let channels = ref(msState.channels);
      // for (i in currLen+1 to newCount){
      //   channels := List.append(
      //     channels^, [{ ordinal: i, filter: "Test-" ++ string_of_int(i)}]);
      // }
      // let newState = {...msState, channels: channels^  };
      // setState(_=>newState);
      // newState;

      // Method 2: create a new array of the correct size and fill it
      // then convert back to a list.
      let newChannels = Array.make(newCount, {ordinal:0, filter:"x"});

      // Copy over the old channels
      msState.channels 
        |> List.iteri((i,channel)=>newChannels[i]=channel);

      // Add the extra channels
      for (i in currLen to newCount-1){
        let newOrdinal = i-currLen + maxOrdinal + 1;
        newChannels[i] = { 
          ordinal: newOrdinal, 
          filter: "Test-" ++ string_of_int(newOrdinal)
        };
      }

      let newState = {...msState, channels: Array.to_list(newChannels)}
      setState(_=>newState);
      newState;

    } else {
      msState;
    }
  };

  let formSubmit = (event) => {
    ReactEvent.Form.preventDefault(event);
    let newState = updateChannelCount(channelCount);
    handleSubmit(_=>newState);
  };

  let addChannel = (event) => {
    ReactEvent.Synthetic.preventDefault(event);
    let maxOrdinal = msState.channels 
      |> List.fold_left((max,channel) => channel.ordinal>max ? channel.ordinal : max, 0);
    let newState = {
      ...msState, channels: msState.channels 
        @ [{ ordinal: maxOrdinal+1, filter: "Test-" ++ string_of_int(maxOrdinal+1)}]
    }
    setState(_=>newState);
    setChannelCount(_=>List.length(newState.channels));
  };

  let removeChannel = (ordinal) => {
    let newState = {
      ...msState, channels: msState.channels |> List.filter(channel=>channel.ordinal!=ordinal)
    };
    setState(_=>newState);
    setChannelCount(_=>List.length(newState.channels));
  };

  let updateValue = (field, event) => {
    // Note: must cache the value because setState will run async to event
    let value = event->getValue;
    setState( _ => switch(field) {
        | Name => { ...msState, name: value } 
        | Mag => { ...msState, magnification: value }
      } 
    );
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
      <button type_="submit"  className="mdc-button mdc-button--raised mdc-button--dense" >(str("submit"))</button>
    </form>
    <hr />
    <ChannelForm channels={msState.channels} removeChannel />
    <button id="add_channel_button" 
      onClick=addChannel 
      className="mdc-button mdc-button--raised mdc-button--dense" >
      <span className="mdc-button__label">(str("Add"))</span>
    </button>
  </div>
}