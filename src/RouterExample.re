open Common;

// type action = 
//   | ChangeRoute(route)
//   | UpdateProjectForm(projectState)
//   | UpdateMsForm(microscope)

// TODO: Initial States come from the server

let initialProjectState: projectState = {
  projectName: "p123456",
  pi: "Jill PI",
  primaryIrb: "IRB-pri",
  lspIrb: "IRB-lsp",
  protocolIo: "protocol io 1",
  scientistFrom: "Scientist From",
  pathologyCore: "Path core",
  coPi: Some("Jane CoPI"),
  scientistConducting: "Joe Scientist",
  grant: "Grant-1",
  purpose: "Purpose-1",
  organism: "Org-1",
  sampleSize: None,
  cycles: "4",
  comments: "Test comment here..."
};

module BMS = Belt.Map.String;
let initialMsState = BMS.getExn(microscopeMap, "MS1");

let mapUrlToRoute = (url: ReasonReact.Router.url) => 
  switch(url.path) {
    | ["project"] => Project
    | ["microscope"] => Microscope
    // | ["planned"] => Planned
    // | ["actual"] => Actual
    | _ => Project
  };


[@react.component]
let make = () => {

  let (route, setRoute) = React.useState(
    ()=>mapUrlToRoute(ReasonReactRouter.dangerouslyGetInitialUrl()));
  
  let (projectState, setProjectState) = React.useState(
    ()=>initialProjectState);
  
  let (msState, setMsState) = React.useState(
    ()=>initialMsState);

  // let (modalState, setModalState) = React.useState(()=>{
  //     callBackOk: (_)=>(),
  //     callBackCancel: (_)=>(),
  //     shown: false,
  //     message: "",
  //   });

  let (modalState, dispatchModal) = React.useReducer(
    (state, action)=>
      switch(action) {
        | Hide => { ...state, shown: false}
        | Show(message, callBackOk, callBackCancel) =>
            { message, shown: true, callBackOk, callBackCancel }
      },
      {
        message: "", shown: false, 
        callBackOk: (_)=>(), callBackCancel: (_)=>()
      }
  );

  // let showModalMessage = (message, ~shown, ~callBackOk, ~callBackCancel)  => {

  //   let finalCancel = (message) => {
  //     setModalState(_=>{...modalState, shown: false});
  //     Js.log("parent cancel...");
  //     callBackCancel(message);
  //   };
  //   let finalOk = (message) => {
  //     setModalState(_=>{...modalState, shown: false});
  //     Js.log("parent ok...");
  //     callBackOk(message);
  //   };

  //   setModalState(_=>{
  //     message: message,
  //     shown: shown,
  //     callBackCancel: finalCancel,
  //     callBackOk: finalOk
  //   });

  // };

  let toggleModal = event => {
    // This is an example only!
    ReactEvent.Synthetic.preventDefault(event);
    dispatchModal(
      Show("Test message!",
        (msg)=>{
          Js.log2("Test callBackOk", msg);
          dispatchModal(Hide);
        },
        (msg)=>{
          Js.log2("Test callBackCancel", msg);
          dispatchModal(Hide);
        })
      );
    // showModalMessage(
    //   "Test message!",
    //   ~shown=!modalState.shown,
    //   ~callBackOk=(msg)=>{
    //     Js.log2("Callback ok: ", msg);
    //   }, 
    //   ~callBackCancel=(msg)=>{
    //     Js.log2("Callback cancel:", msg);
    //   }
    // );
  };

  let handleMsUpdate = (newMsState) => {
    setMsState(_=>newMsState);

    // showModalMessage(
    //   "Really update?",
    //   ~shown=true,
    //   ~callBackCancel=(message) => {
    //     Js.log2("Cancel...", message);
    //     // NOTE: this won't work, because react will bail out of the re-render
    //     // if the state has not changed.
    //     // setMsState(_=>msState)
    //     setModalState(_=>{...modalState, shown: false});
    //   },
    //   ~callBackOk=(message) => {
    //     Js.log(message);
    //     setMsState(_=>newMsState);
    //     setModalState(_=>{...modalState, shown: false});
    //   }
    // );
  };

  React.useEffect(() => {
    let watcherId = ReasonReactRouter.watchUrl(url=>setRoute( _ => url |> mapUrlToRoute));

    Some(() => {
      Js.log("apply Effect");
      ReasonReactRouter.unwatchUrl(watcherId);
    });
    // Some(() => ReasonReactRouter.unwatchUrl(watcherId));
  });


  <div >
    <div>(str("Project Name: " ++ projectState.projectName))</div>
    <Modal 
      callBackOk=modalState.callBackOk
      callBackCancel=modalState.callBackCancel
      customClass="foo" 
      show=modalState.shown >
      (str(modalState.message))
    </Modal>
    <div className="wrapper">
    <div>
    <ul>
      <li><Link href="project" selected=((route == Project)) >(str("project"))</Link></li>
      <li><Link href="microscope" selected=((route == Microscope)) >(str("microscope"))</Link></li>
      // <li><Link href="planned" selected=((self.state.route == Planned)) >(str("planned"))</Link></li>
      // <li><Link href="actual" selected=((self.state.route == Actual)) >(str("actual"))</Link></li>
    </ul>
    <button onClick=(toggleModal) > (str(modalState.shown ? "hide": "show modal")) </button>
    </div>
    (
      {
        switch(route) {
          | Project =>
            <ProjectForm 
              handleSubmit=setProjectState
              key="project_form"
              initialState=projectState />
          | Microscope => 
            <MicroscopeForm 
              handleSubmit=handleMsUpdate
              key="ms_form"
              initialState=msState
              dispatchModal />
          // | Planned =>
          //   <SimpleForm 
          //     handleSubmit
          //     key="planned_form"
          //     initialState=self.state.values />
          // | Actual =>
          //   <SimpleForm 
          //     handleSubmit
          //     key="actual_form"
          //     initialState=self.state.values />
        }
      }
    )
  </div>
  </div>
};

