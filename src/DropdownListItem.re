[%raw {|require('flag-icon-css/css/flag-icon.min.css')|}];

let component = ReasonReact.statelessComponent("DropdownListItem");

let make = (~country: FetchCountries.country, ~onClick, _children) => {
  let handleClick = _event => onClick(country);

  {
    ...component,
    render: _self => {
      let countryCode = country.value;

      <div onClick=handleClick>
        <span className={j|flag-icon flag-icon-$countryCode|j} />
        (ReasonReact.string(country.label))
      </div>;
    },
  };
};