
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
let make = (~initialState: microscope, ~handleSubmit, ~showModalMessage )=>{

  let (msState, setState) = React.useState(()=>initialState);
  let (originalState, setOriginalState) = React.useState(_=>initialState);
  let (channelCount, setChannelCount) = React.useState(()=>List.length(msState.channels));

  let updateChannels = (newCount, channels) => {

    let currLen = List.length(channels);
    if (newCount < currLen) {
        channels
          |> ListLabels.fold_left( 
            ~f=(newList,channel) =>
              List.length(newList) < newCount ? newList @ [channel] : newList,
            ~init=[]);
    } else if (newCount > currLen) {
      
      // Create a larger list and populate it

      let maxOrdinal = channels |> List.fold_left(
          (max,channel) => channel.ordinal>max ? channel.ordinal : max, 0);
      let newChannels = Array.make(newCount, {ordinal:0, filter:""});
      channels |> List.iteri(
        (i,channel)=>newChannels[i]=channel );
      
      // Add the extra channels
      
      for (i in 0 to newCount-currLen-1){
        let newOrdinal = maxOrdinal + 1 + i;
        newChannels[currLen+i] = {
          ordinal: newOrdinal,
          filter: "Filter-" ++ string_of_int(newOrdinal)
        };
      }
      Array.to_list(newChannels);
    } else {
      channels;
    }
  };

  let formSubmit = (event) => {
    
    ReactEvent.Form.preventDefault(event);
    
    // For Demo only: doing two things at once:
    // - updating the state (if the channelCount changes)
    // - calling handleSubmit.
    
    let newState = {
        ...msState, 
        channels: msState.channels |> updateChannels(channelCount) };
    if (newState != originalState || newState.channels != originalState.channels){
      showModalMessage(
        "Really update?",
        ~shown=true,
        ~callBackOk=(message) => {
          Js.log(message);
          setState(_=>newState);
          setOriginalState(_=>newState);   
          handleSubmit(newState);
        },
        ~callBackCancel=(message) => {
          Js.log2("Cancel bail out!...", message);
          setState(_=>originalState);
          setChannelCount(_=>List.length(originalState.channels));
        },
      );
    } else {
      setState(_=>newState);    
      handleSubmit(newState);
    }

    // setState(_=>newState);    
    // handleSubmit(newState);

  };

  let addChannel = (event) => {
    
    ReactEvent.Synthetic.preventDefault(event);
    
    let maxOrdinal = msState.channels 
      |> List.fold_left(
        (max,channel) => channel.ordinal>max ? channel.ordinal : max, 0);
    let newState = {
      ...msState, channels: msState.channels 
        @ [{ ordinal: maxOrdinal+1, filter: "Test-" ++ string_of_int(maxOrdinal+1)}]
    }
    setState(_=>newState);
    setChannelCount(_=>List.length(newState.channels));
  };

  let removeChannel = (ordinal) => {
    
    let newState = {
      ...msState, 
      channels: msState.channels 
        |> List.filter(channel=>channel.ordinal!=ordinal)
    };
    setState(_=>newState);
    setChannelCount(_=>List.length(newState.channels));
  };
  
  let updateValue = (field, event) => {
    
    // Note: cache value as setState will run async
    let value = event->getValue;
    
    let newState = switch(field) {
        | Name => {
          // TODO: warn before updating with the default value from mapping
          Belt.Map.String.getExn(microscopeMap, value)
        }
        | Mag => { ...msState, magnification: value }
      };
    setState(_=>newState);
    setChannelCount(_=>List.length(newState.channels));
  };

  let microscopeOptions = microscopes => {
    
    microscopes
    |> List.map( microscope =>{
      <option key=("ms-"++microscope.name) 
        value=microscope.name >(str(microscope.name))</option>
    })
    |> Array.of_list
    |> ReasonReact.array;
  };
  
  <div>
    <form className="data_list_form" onSubmit=formSubmit >
      <label htmlFor="name">(str("Name"))</label>
      <select id="name" style=(
          ReactDOMRe.Style.make(~height="21px", ()))
        className="mdc-select" 
        value={msState.name}
        onChange=updateValue(Name)
      >
        (microscopeOptions(storedMicroscopes))
      </select>
      <label htmlFor="channels">(str("Channels:"))</label>
      <input id="channels" value=string_of_int(channelCount) 
        onChange=(e=>setChannelCount(e->getValue)) />
      <label htmlFor="mag" >(str("Magnification:"))</label>
      <input id="mag" value={msState.magnification} 
        onChange=updateValue(Mag)
      />
      <button type_="submit" className="mdc-button mdc-button--raised mdc-button--dense" >(str("submit"))</button>
    </form>
    <hr />
    <ChannelForm channels={msState.channels} removeChannel />
    <button id="add_channel_button" 
      onClick=addChannel 
      className="mdc-button mdc-button--raised mdc-button--dense" >
      <span className="mdc-button__label">(str("Add"))</span>
    </button>
  </div>
};
