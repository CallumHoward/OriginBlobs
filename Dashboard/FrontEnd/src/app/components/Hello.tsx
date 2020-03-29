import * as React from "react";
import DeviceTable from "./DeviceTable";
import Section from "./Section";
import { Container, Row, Col } from "reactstrap";

export const Hello = () => {
  return (
    <Container>
      <Row>
        <h1>Dashboard</h1>
      </Row>
      <Row>
        <Col>
          <DeviceTable />
        </Col>
      </Row>
    </Container>
  );
};
