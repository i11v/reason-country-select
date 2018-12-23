type country = {
  label: string,
  value: string,
};

type requestState =
  | Init
  | Loading
  | Failure
  | Success(list(country));

module Decode = {
  let country = country =>
    Json.Decode.{
      label: field("label", string, country),
      value: field("value", string, country),
    };
  let countries = json : list(country) => Json.Decode.list(country, json);
};

let url = "https://gist.githubusercontent.com/rusty-key/659db3f4566df459bd59c8a53dc9f71f/raw/4127f9550ef063121c564025f6d27dceeb279623/counties.json";

let fetchCountries = () =>
  Js.Promise.(
    Fetch.fetch(url)
    |> then_(Fetch.Response.json)
    |> then_(json =>
         json |> Decode.countries |> (countries => Some(countries) |> resolve)
       )
    |> catch(_err => resolve(None))
  );