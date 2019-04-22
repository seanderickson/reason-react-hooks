open Common;

[%bs.raw {|require('./modal.css')|}];

[@react.component]
let make = (~show, ~customClass, ~callBackOk, ~callBackCancel, ~children) => {

  let classes = "modal " ++ customClass; 

  <div className=classes style=(
          ReactDOMRe.Style.make(~display=show ? "block" : "none", ())) >
    <div className="overlay" onClick={_=>callBackCancel("Cancel")}></div>
    <div className="modal_content">
      <div >
      children
      </div>
      <button key="cancel" title="Close" className="close_modal"  onClick={_=>callBackCancel("Cancel")} >
        (str("Cancel"))
      </button>
      <button key="ok" title="Ok" className="close_modal" onClick=(_=>callBackOk("Ok"))>
        (str("Ok"))
      </button>
    </div>
  </div>
  
}