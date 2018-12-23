ReactDOMRe.renderToElementWithId(
  <CountrySelect
    className="custom-class"
    country=(Some("us"))
    onChange=(country => Js.log(country))
  />,
  "root",
);