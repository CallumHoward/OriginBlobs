import * as React from "react";
import DeviceTable from "./DeviceTable";
import Section from "./Section";
import { Container, Row, Col } from "reactstrap";

export const Hello = () => {
  const [deviceData, setDeviceData] = React.useState([]);
  return (
    <div style={{ backgroundColor: "#eee", height: "100vh", width: "100vw" }}>
      <Container>
        <Row>
          <Col>
            <Section>
              <h1>Dashboard</h1>
            </Section>
          </Col>
        </Row>
        <Row>
          <Col>
            <Section>
              <DeviceTable deviceData={deviceData} />
            </Section>
          </Col>
        </Row>
      </Container>
    </div>
  );
};
