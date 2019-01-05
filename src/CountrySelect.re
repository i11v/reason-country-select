type state = {
  countriesRequestStatus: FetchCountries.requestState,
  countries: list(FetchCountries.country),
  selectedCountry: string,
  dropdownVisible: bool,
};

type action =
  | RequestCountries
  | RequestCountriesSuccess(list(FetchCountries.country))
  | RequestCountriesFailure
  | ToggleDropdownVisibility
  | CountrySelected(string);

let component = ReasonReact.reducerComponent("CountrySelect");

module Styles = {
  open Css;

  let container = style([position(`relative)]);

  let dropdown =
    style([
      position(`absolute),
      top(pct(100.0)),
      left(px(-1)),
      margin3(~top=px(2), ~h=zero, ~bottom=zero),
      zIndex(10),
    ]);

  let overlay = visible =>
    style([
      display(visible ? `block : `none),
      position(`fixed),
      width(`percent(100.0)),
      height(`percent(100.0)),
      top(zero),
      left(zero),
      background(transparent),
      zIndex(1),
    ]);
};

let make = (~className, ~country: option(string), ~onChange, _children) => {
  let buttonClick = (event, self) => {
    event->ReactEvent.Mouse.preventDefault;
    self.ReasonReact.send(ToggleDropdownVisibility);
  };

  let countryChange = (country: FetchCountries.country, self) => {
    onChange(country.label);
    self.ReasonReact.send(CountrySelected(country.value));
  };

  {
    ...component,

    didMount: self => self.send(RequestCountries),

    initialState: () => {
      countriesRequestStatus: Init,
      countries: [],
      selectedCountry:
        switch (country) {
        | None => ""
        | Some(text) => text
        },
      dropdownVisible: false,
    },

    reducer: (action, state) =>
      switch (action) {
      | RequestCountries =>
        ReasonReact.UpdateWithSideEffects(
          {...state, countriesRequestStatus: Loading},
          (
            self =>
              Js.Promise.(
                FetchCountries.fetchCountries()
                |> then_(result =>
                     switch (result) {
                     | Some(countries) =>
                       resolve(
                         self.send(RequestCountriesSuccess(countries)),
                       )
                     | None => resolve(self.send(RequestCountriesFailure))
                     }
                   )
                |> ignore
              )
          ),
        )
      | RequestCountriesSuccess(countries) =>
        ReasonReact.Update({
          ...state,
          countries,
          countriesRequestStatus: Success(countries),
        })
      | RequestCountriesFailure =>
        ReasonReact.Update({...state, countriesRequestStatus: Failure})
      | ToggleDropdownVisibility =>
        ReasonReact.Update({
          ...state,
          dropdownVisible: !state.dropdownVisible,
        })
      | CountrySelected(value) =>
        ReasonReact.Update({...state, selectedCountry: value})
      },

    render: self => {
      let {
        countriesRequestStatus,
        countries,
        selectedCountry,
        dropdownVisible,
      } =
        self.state;
      let buttonLabel =
        switch (
          countries
          |> List.find((item: FetchCountries.country) =>
               item.value === selectedCountry
             )
        ) {
        | item => item.label
        | exception Not_found => "Click here!"
        };
      let buttonDisabled =
        switch (countriesRequestStatus) {
        | Success(_countries) => false
        | _ => true
        };

      <div className={Css.merge([className, Styles.container])}>
        <Button disabled=buttonDisabled onClick={self.handle(buttonClick)}>
          {ReasonReact.string(buttonLabel)}
        </Button>
        {
          dropdownVisible ?
            <DropdownList
              className=Styles.dropdown
              options=countries
              onChange={self.handle(countryChange)}
            /> :
            ReasonReact.null
        }
        <div
          className={Styles.overlay(dropdownVisible)}
          onClick={self.handle(buttonClick)}
        />
      </div>;
    },
  };
};