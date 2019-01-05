let component = ReasonReact.statelessComponent("IconTriangle");

let make = _children => {
  ...component,
  render: _self =>
    <svg width="8" height="8" viewBox="0 0 8 8" fill="none">
      <path
        fillRule="evenodd"
        clipRule="evenodd"
        d="M0 2H8L4 7L0 2Z"
        fill="#333333"
      />
    </svg>,
};