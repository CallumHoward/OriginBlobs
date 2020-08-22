// Import React dependencies.
import React, { useEffect, useMemo, useState } from "react";
// Import the Slate editor factory.
import { createEditor } from "slate";

// Import the Slate components and React plugin.
import { Slate, Editable, withReact } from "slate-react";

const App: React.FC = () => {
  // Create a slate editor object that won't change across renders.
  const editor = useMemo(() => withReact(createEditor()), []);

  // Keep track of state for the value of the editor
  const [value, setValue] = useState([
    {
      type: 'paragraph',
      children: [{ text: 'A line of text in a paragraph.' }],
    },
  ]);

  // Render the Slate context.
  return (
    <Slate editor={editor} value={value} onChange={(opts) => setValue(opts.value)}>
      <Editable />
    </Slate>
  );
};

export default App;
