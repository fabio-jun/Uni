import React, { useState, useEffect, useRef } from 'react';
import { View, Text, StyleSheet, SafeAreaView, StatusBar, ScrollView, TextInput, TouchableOpacity, Alert, Keyboard } from 'react-native';
import { colors } from "./constants";

const tries = 6;

const Termo = ({ words }) => {
  const [word, setWord] = useState('');
  const [letters, setLetters] = useState([]);
  const [rows, setRows] = useState(new Array(tries).fill(new Array(5).fill("")));
  const [curRow, setCurRow] = useState(0);
  const [curCol, setCurCol] = useState(0);
  const [gameState, setGameState] = useState("playing");
  const [inputValue, setInputValue] = useState('');
  const textInputRef = useRef(null);

  useEffect(() => {
    resetGame();
  }, [words]);

  const getRandomWord = () => {
    const randomIndex = Math.floor(Math.random() * words.length);
    return words[randomIndex];
  };

  const resetGame = () => {
    const randomWord = getRandomWord();
    setWord(randomWord);
    setLetters(randomWord.split(''));
    setRows(new Array(tries).fill(new Array(randomWord.length).fill("")));
    setCurRow(0);
    setCurCol(0);
    setGameState("playing");
    setInputValue('');
  };

  useEffect(() => {
    if (curRow > 0) {
      checkGameState();
    }
  }, [curRow]);

  const checkGameState = () => {
    if (checkIfWon()) {
      Alert.alert("Vitória!", "Parabéns, você acertou a palavra!", [{ text: "OK", onPress: resetGame }]);
      setGameState("won");
    } else if (checkIfLost()) {
      Alert.alert("Game Over!", `A palavra correta era: ${word}`, [{ text: "OK", onPress: resetGame }]);
      setGameState("lost");
    }
  };

  const checkIfWon = () => {
    const row = rows[curRow - 1];
    return row.every((letter, i) => letter === letters[i]);
  };

  const checkIfLost = () => {
    return curRow === rows.length;
  };

  const onChangeText = (text) => {
    if (gameState !== "playing") {
      return;
    }

    if (text.length < inputValue.length) {
      onKeyPressed('Backspace');
    } else {
      const key = text[text.length - 1];
      onKeyPressed(key);
    }
    setInputValue(text);
  };

  const onKeyPressed = (key) => {
    const updatedRows = copyArray(rows);

    if (key === 'Backspace') {
      if (curCol > 0) {
        updatedRows[curRow][curCol - 1] = "";
        setRows(updatedRows);
        setCurCol(curCol - 1);
      }
      return;
    }

    if (key === 'Enter') {
      if (curCol === rows[0].length) {
        setCurRow(curRow + 1);
        setCurCol(0);
      }
      return;
    }

    if (key.length === 1) {
      if (curCol < rows[0].length) {
        updatedRows[curRow][curCol] = key.toUpperCase();
        setRows(updatedRows);
        setCurCol(curCol + 1);
      }
    }
  };

  const isCellActive = (row, col) => {
    return row === curRow && col === curCol;
  };

  const getCellColor = (letter, row, col) => {
    if (row >= curRow) {
      return colors.black;
    }

    if (letter === letters[col]) {
      return colors.primary;
    }
    if (letters.includes(letter)) {
      return colors.secondary;
    }
    return colors.darkgrey;
  };

  const handleCellPress = (rowIndex, colIndex) => {
    setCurRow(rowIndex);
    setCurCol(colIndex);
    if (textInputRef.current) {
      Keyboard.dismiss();
      setTimeout(() => {
        textInputRef.current.focus();
      }, 100);
    }
  };

  const copyArray = (arr) => {
    return [...arr.map((rows) => [...rows])];
  };

  return (
    <SafeAreaView style={styles.container}>
      <StatusBar style="light" />
      <ScrollView style={styles.map}>
        {rows.map((row, i) => (
          <View key={`row-${i}`} style={styles.row}>
            {row.map((letter, j) => (
              <TouchableOpacity 
                key={`cell-${i}-${j}`}
                style={[
                  styles.cell, 
                  { 
                    borderColor: isCellActive(i, j) 
                      ? colors.lightgrey 
                      : colors.darkgrey,
                    backgroundColor: getCellColor(letter, i, j),
                  },
                ]}
                onPress={() => handleCellPress(i, j)}
              >
                <Text style={styles.cellText}>{letter.toUpperCase()}</Text>
              </TouchableOpacity>
            ))}
          </View>
        ))}
      </ScrollView>
      <TextInput
        ref={textInputRef}
        style={styles.hiddenInput}
        value={inputValue}
        onChangeText={onChangeText}
        autoFocus
        blurOnSubmit={false}
        keyboardType="default"
        onSubmitEditing={() => onKeyPressed('Enter')}
      />
    </SafeAreaView>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: colors.background,
  },
  map: {
    alignSelf: 'stretch',
    height: 100,
  },
  row: {
    alignSelf: 'stretch',
    flexDirection: 'row',
    justifyContent: "center",
  },
  cell: {
    borderWidth: 3,
    borderColor: colors.darkgrey,
    flex: 1,
    maxWidth: 70,
    aspectRatio: 1,
    margin: 3,
    justifyContent: 'center',
    alignItems: 'center',
  },
  cellText: {
    color: colors.lightgrey,
    fontWeight: 'bold',
    fontSize: 30,
  },
  hiddenInput: {
    width: 1,
    height: 1,
    opacity: 0,
  },
});

export default Termo;
