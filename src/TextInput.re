type state = {text: string};

type action =
  | InputChange(string);

let component = ReasonReact.reducerComponent("TextInput");

let make =
    (
      ~className="input",
      ~placeholder="",
      ~onChange=(_v: string) => (),
      _children,
    ) => {
  ...component,

  initialState: () => {text: ""},

  reducer: (action, state) =>
    switch (action) {
    | InputChange(text) => ReasonReact.Update({...state, text})
    },

  render: self => {
    let change = (event, self) => {
      let text = event->ReactEvent.Form.target##value;
      self.ReasonReact.send(InputChange(text));
      onChange(text);
    };

    <input
      className
      placeholder
      type_="text"
      value={self.state.text}
      onChange={self.handle(change)}
    />;
  },
};