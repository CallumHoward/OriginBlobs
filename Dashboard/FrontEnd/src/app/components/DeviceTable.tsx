import * as React from "react";
import styled from 'styled-components';
import { Table } from 'reactstrap';

const DeviceTable = () => (
  <>
    <Table>
      <thead>
        <tr>
          <th>ID</th>
          <th>Name</th>
          <th>Type</th>
          <th>Last Contact</th>
          <th>Battery</th>
          <th>Version</th>
          <th>Status</th>
        </tr>
      </thead>
      <tbody>
        <tr>
          <td>Stacey</td>
          <td></td>
          <td></td>
          <td></td>
          <td></td>
          <td></td>
          <td></td>
        </tr>
        <tr>
          <td>Monica</td>
          <td></td>
          <td></td>
          <td></td>
          <td></td>
          <td></td>
          <td></td>
        </tr>
        <tr>
          <td>Jessica</td>
          <td></td>
          <td></td>
          <td></td>
          <td></td>
          <td></td>
          <td></td>
        </tr>
      </tbody>
    </Table>
  </>
);

 export default DeviceTable;
