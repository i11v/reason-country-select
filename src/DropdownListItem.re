[%raw {|require('flag-icon-css/css/flag-icon.min.css')|}];

let component = ReasonReact.statelessComponent("DropdownListItem");

module Styles = {
  open Css;

  let container = style([display(`flex), padding2(~v=px(4), ~h=px(10))]);

  let icon =
    style([
      width(px(14)),
      margin4(~top=zero, ~right=px(8), ~bottom=zero, ~left=zero),
    ]);

  let label =
    style([
      color(Theme.Colors.n800),
      fontFamily(Theme.fontFamilySans),
      fontSize(Theme.fontSizeSM),
      lineHeight(Theme.fontSizeMD),
      overflow(`hidden),
      whiteSpace(`nowrap),
      textOverflow(`ellipsis),
    ]);
};

let make =
    (~className="", ~country: FetchCountries.country, ~onClick, _children) => {
  let handleClick = _event => onClick(country);

  {
    ...component,
    render: _self => {
      let countryCode = country.value;

      <div
        className=(Css.merge([className, Styles.container]))
        onClick=handleClick>
        <span
          className=(
            Css.merge([{j|flag-icon flag-icon-$countryCode|j}, Styles.icon])
          )
        />
        <span className=Styles.label>
          (ReasonReact.string(country.label))
        </span>
      </div>;
    },
  };
};