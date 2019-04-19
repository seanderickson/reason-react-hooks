open Common

let printChannels = channels => {
  channels
  |> List.map( channel => {
    <div key=(string_of_int(channel.ordinal)) className="channel_form" >
      <div className="channel_item">(str(string_of_int(channel.ordinal)))</div>
      <div className="channel_item">(str(channel.filter))</div>
    </div>
  })
  |> Array.of_list
  |> ReasonReact.array;
};

[@react.component]
let make = (~channels: channels, ~handleSubmit )=> {

  // let formSubmit = (event) => {
  //   ReactEvent.Form.preventDefault(event);
  //   handleSubmit(_=>channels);
  // };

  <div id="channels">
    <h3>(str("Channels:"))</h3>
    <form className="" id="channels" onSubmit=handleSubmit>
      (printChannels(channels))
    </form>
  </div>
};