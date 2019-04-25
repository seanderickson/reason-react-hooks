open Common;

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

  let (modalState, dispatchModal) = React.useReducer(
    (state, action)=>
      switch(action) {
        | Hide => { ...state, shown: false}
        | Show(message, callBackOk, callBackCancel) =>
            { message, shown: true, callBackOk, callBackCancel }
        | ModalCancel => {
            state.callBackCancel("Cancel");
            { ...state, shown: false };
          }
        | ModalOk => {
            state.callBackOk("ok");
            {...state, shown: false };
        }
      },
      {
        message: "", shown: false, 
        callBackOk: (_)=>(), callBackCancel: (_)=>()
      }
  );

  let handleMsUpdate = (newMsState) => {
    setMsState(_=>newMsState);
  };

  React.useEffect(() => {
    let watcherId = ReasonReactRouter.watchUrl(url=>setRoute( _ => url |> mapUrlToRoute));

    Some(() => {
      Js.log("apply Effect");
      ReasonReactRouter.unwatchUrl(watcherId);
    });
  });

  <div >
    <div>(str("Project Name: " ++ projectState.projectName))</div>
    <Modal 
      // callBackOk=modalState.callBackOk
      // callBackCancel=modalState.callBackCancel
      dispatchModal
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
              handleSubmit=setMsState
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
