type state = {
  focused: bool,
  text: string,
};

type action =
  | Focus
  | Blur
  | InputChange(string);

let component = ReasonReact.reducerComponent("TextInput");

module Styles = {
  open Css;

  let container = style([display(`flex), transition(~duration=300, ~timingFunction=`easeInOut, "box-shadow")]);

  let icon = focused =>
    style([
      padding4(~top=px(10), ~right=zero, ~bottom=zero, ~left=px(10)),
      color(focused ? rgba(0, 100, 255, 0.85) : Theme.Colors.n800),
      transition(~duration=300, ~timingFunction=`easeInOut, "color"),
    ]);

  let input =
    style([
      width(`percent(100.0)),
      height(`percent(100.0)),
      margin(zero),
      padding2(~v=px(4), ~h=px(8)),
      borderWidth(zero),
      boxSizing(`borderBox),
      fontFamily(Theme.fontFamilySans),
      fontSize(Theme.fontSizeSM),
      lineHeight(Theme.fontSizeMD),
      background(transparent),
      selector(":focus", [outlineStyle(`none)]),
    ]);
};

let make = (~className="", ~placeholder="", ~onChange=(_v: string) => (), _children) => {
  ...component,

  initialState: () => {focused: false, text: ""},

  reducer: (action, state) =>
    switch (action) {
    | Focus => ReasonReact.Update({...state, focused: true})
    | Blur => ReasonReact.Update({...state, focused: false})
    | InputChange(text) => ReasonReact.Update({...state, text})
    },

  render: self => {
    let change = (event, self) => {
      let text = event->ReactEvent.Form.target##value;
      self.ReasonReact.send(InputChange(text));
      onChange(text);
    };

    <div className={Css.merge([className, Styles.container])}>
      <span className={Styles.icon(self.state.focused)}> <IconMagnifier /> </span>
      <input
        className=Styles.input
        placeholder
        type_="text"
        value={self.state.text}
        onFocus={_event => self.send(Focus)}
        onBlur={_event => self.send(Blur)}
        onChange={self.handle(change)}
      />
    </div>;
  },
};
