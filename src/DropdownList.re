type state = {
  filteredCountries: list(FetchCountries.country),
  text: string,
};

type action =
  | InputChange(string);

let component = ReasonReact.reducerComponent("Dropdown");

module Styles = {
  open Css;

  let container =
    style([
      width(px(230)),
      height(px(200)),
      border(px(1), solid, rgba(0, 0, 0, 0.08)),
      background(white),
      borderRadius(px(2)),
    ]);

  let searchInputHeight = px(35);
  let searchInput =
    style([
      width(pct(100.0)),
      height(searchInputHeight),
      borderBottom(px(1), solid, rgba(0, 0, 0, 0.08)),
    ]);

  let countryList =
    style([
      width(pct(100.0)),
      height(`calc((`sub, `percent(100.0), searchInputHeight))),
      padding2(~v=px(4), ~h=zero),
      boxSizing(`borderBox),
      overflow(`scroll),
    ]);
};

let make = (~className="", ~options, ~onChange, _children) => {
  ...component,
  initialState: () => {filteredCountries: options, text: ""},
  reducer: (action, _state) =>
    switch (action) {
    | InputChange(text) =>
      ReasonReact.Update({
        filteredCountries:
          options
          |> List.filter((country: FetchCountries.country) =>
               Js.Re.fromStringWithFlags(text, ~flags="i")
               |> Js.Re.test(country.label)
             ),
        text,
      })
    },
  render: self => {
    let {filteredCountries} = self.state;

    <div className=(Css.merge([className, Styles.container]))>
      <TextInput
        className=Styles.searchInput
        placeholder="Search"
        onChange=(text => self.send(InputChange(text)))
      />
      <div className=Styles.countryList>
        (
          filteredCountries
          |> List.map((country: FetchCountries.country) =>
               <DropdownListItem key=country.value country onClick=onChange />
             )
          |> Array.of_list
          |> ReasonReact.array
        )
      </div>
    </div>;
  },
};