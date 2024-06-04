package br.uel.administrador;

import jakarta.validation.Valid;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;

import java.util.List;

@Controller
public class ItemCardapioController {

    @Autowired
    ItemCardapioRepository itemCardapioRepository;

    @Autowired
    RestauranteRepository restauranteRepository;


    @GetMapping("/cardapio/{id}")
    public String mostrarCardapioRestaurante(@PathVariable("id") int id, Model model) {
        List<ItemCardapio> itensCardapio = itemCardapioRepository.findByRestauranteId(id);
        Restaurante restaurante = restauranteRepository.findById(id).orElse(null);
        assert restaurante != null;
        model.addAttribute("itensCardapio", itensCardapio);
        model.addAttribute("restauranteNome", restaurante.getNome());
        model.addAttribute("restauranteId", id);
        return "cardapio";
    }

    @GetMapping("/novo-item/{restauranteId}")
    public String mostrarFormNovoItem(@PathVariable("restauranteId") int restauranteId, ItemCardapio itemCardapio, Model model) {
        model.addAttribute("restauranteId", restauranteId);
        return "novo-item";
    }


    @PostMapping("/adicionar-item")
    public String adicionarItem(@Valid ItemCardapio itemCardapio,
                                BindingResult result,
                                @RequestParam("restauranteId") int restauranteId) {
        if (result.hasErrors()) {
            return "novo-item";
        }

        Restaurante restaurante = restauranteRepository.findById(restauranteId)
                .orElseThrow(() -> new IllegalArgumentException("O id do restaurante é inválido:" + restauranteId));


        itemCardapio.setRestaurante(restaurante);
        itemCardapioRepository.save(itemCardapio);

        return "redirect:/cardapio/" + restauranteId;
    }

    @GetMapping("/editar-item/{id}")
    public String mostrarFormAtualizarItem(@PathVariable("id") int id, Model model) {
        ItemCardapio itemCardapio = itemCardapioRepository.findById(id)
                .orElseThrow(() -> new IllegalArgumentException("Item do cardápio não encontrado para o ID: " + id));
        model.addAttribute("itemCardapio", itemCardapio);
        return "atualizar-item";
    }

    @PostMapping("/atualizar-item/{id}")
    public String atualizarItem(@PathVariable("id") int id, @Valid ItemCardapio itemCardapio,
                                BindingResult result, Model model) {
        if (result.hasErrors()) {
            itemCardapio.setId(id);
            return "atualizar-item";
        }

        ItemCardapio existingItem = itemCardapioRepository.findById(id)
                .orElseThrow(() -> new IllegalArgumentException("Item do cardápio não encontrado para o ID: " + id));

        existingItem.setNome(itemCardapio.getNome());
        existingItem.setDescricao(itemCardapio.getDescricao());
        existingItem.setPreco(itemCardapio.getPreco());

        itemCardapioRepository.save(existingItem);
        return "redirect:/cardapio/" + existingItem.getRestaurante().getId();
    }

    @GetMapping("/remover-item/{id}")
    public String removerItem(@PathVariable("id") int id) {
        ItemCardapio itemCardapio = itemCardapioRepository.findById(id)
                .orElseThrow(() -> new IllegalArgumentException("Item do cardápio não encontrado para o ID: " + id));

        int restauranteId = itemCardapio.getRestaurante().getId();
        itemCardapioRepository.delete(itemCardapio);

        return "redirect:/cardapio/" + restauranteId;
    }


}
