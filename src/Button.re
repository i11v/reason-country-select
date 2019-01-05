let component = ReasonReact.statelessComponent("Button");

module Styles = {
  open Css;

  let container =
    style([
      padding2(~v=px(3), ~h=px(10)),
      border(px(1), solid, Theme.Colors.n200),
      borderRadius(px(3)),
      background(white),
      cursor(`pointer),
      outlineStyle(`none),
    ]);

  let label =
    style([
      margin(zero),
      marginRight(px(7)),
      color(Theme.Colors.n800),
      fontFamily(Theme.fontFamilySans),
      fontSize(Theme.fontSizeSM),
      lineHeight(Theme.fontSizeMD),
    ]);
};
let make = (~disabled, ~onClick, children) => {
  ...component,
  render: _self =>
    <button className=Styles.container disabled onClick>
      <span className=Styles.label> ...children </span>
      <span> <IconTriangle /> </span>
    </button>,
};