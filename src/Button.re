let component = ReasonReact.statelessComponent("Button");

let make = (~disabled, ~onClick, children) => {
  ...component,
  render: _self => <button disabled onClick> ...children </button>,
};