open Common;

type action = 
  | ChangeRoute(route)
  | UpdateProjectForm(projectState)
  | UpdateMsForm(microscopeState)

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

let initialMsState = {
  name: "MS1",
  channels: "4",
  magnification: "20x"
};

// let component = ReasonReact.reducerComponent("RouterApp");

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
  // ...component,
  // initialState: () => { 
  //   route: mapUrlToRoute(ReasonReact.Router.dangerouslyGetInitialUrl()), 
  //   projectState: initialProjectState, 
  //   microscopeState: initialMsState 
  //   },

  let (route, setRoute) = React.useState(()=>mapUrlToRoute(ReasonReactRouter.dangerouslyGetInitialUrl()));
  let (projectState, setProjectState) = React.useState(()=>initialProjectState);
  let (msState, setMsState) = React.useState(()=>initialMsState);

  // let (state, dispatch) = React.useReducer(
  //   (state, action) =>
  //     switch (action) {
  //     | ChangeRoute(route) =>  ReasonReact.Update({ ...state, route: route })
  //     | UpdateProjectForm(newState) => ReasonReact.Update({ ...state, projectState: newState })
  //     | UpdateMsForm(newState) => ReasonReact.Update({...state, microscopeState: newState})
  //     },
  //   {count: 0}
  // );

  // didMount: (self) => {
  //   let watcherId = ReasonReactRouter.watchUrl(
  //     url=>self.send(ChangeRoute(url |> mapUrlToRoute)));
  //   self.onUnmount(() => ReasonReactRouter.unwatchUrl(watcherId));
  // },

  React.useEffect(() => {
    let watcherId = ReasonReactRouter.watchUrl(url=>setRoute(route => url |> mapUrlToRoute));

    Some(() => {
      Js.log("apply Effect");
      ReasonReactRouter.unwatchUrl(watcherId);
    });
    // Some(() => ReasonReactRouter.unwatchUrl(watcherId));
  });

  <div >
    <div>(str("Project Name: " ++ projectState.projectName))</div>
    <div className="wrapper">
    <ul>
      <li><Link href="project" selected=((route == Project)) >(str("project"))</Link></li>
      <li><Link href="microscope" selected=((route == Microscope)) >(str("microscope"))</Link></li>
      // <li><Link href="planned" selected=((self.state.route == Planned)) >(str("planned"))</Link></li>
      // <li><Link href="actual" selected=((self.state.route == Actual)) >(str("actual"))</Link></li>
    </ul>
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
              initialState=msState />
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

