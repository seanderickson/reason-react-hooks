open Common


[@react.component]
let make = (~channels: channels, ~removeChannel, ())=> {

  // let formSubmit = (event) => {
  //   ReactEvent.Form.preventDefault(event);
  //   handleSubmit(_=>channels);
  // };

  let localRemove = (ordinal, event) => {
    ReactEvent.Synthetic.preventDefault(event);
    removeChannel(ordinal);
  };

  let printChannels = channels => {
    channels
    |> List.map( channel => {
      <div key=(string_of_int(channel.ordinal)) className="channel_row" >
        <div className="channel_item">(str(string_of_int(channel.ordinal)))</div>
        <div className="channel_item">(str(channel.filter))</div>
        <button 
          onClick=localRemove(channel.ordinal)
          id="delete_channel"  
          className="mdc-button mdc-button--raised mdc-button--dense"  >(str("Remove"))</button>
      </div>
    })
    |> Array.of_list
    |> ReasonReact.array;
  };

  <div id="channels">
    <h3>(str("Channels:"))</h3>
    <form className="channel_form" id="channels" >
      <div className="channel_row">
        <label>(str("Ordinal"))</label><label>(str("Filter"))</label>
      </div>
      (printChannels(channels))
    </form>
  </div>
};